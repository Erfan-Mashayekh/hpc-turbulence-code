#include "Simulation.hpp"

#include "Solvers/SORSolver.hpp"
#include "Solvers/PetscSolver.hpp"
#include "Solvers/EigenSolver.hpp"

namespace NSEOF {

Simulation::Simulation(Parameters& parameters, FlowField& flowField)
    : parameters_(parameters)
    , flowField_(flowField)
    , maxUStencil_(parameters)
    , maxUFieldIterator_(flowField_, parameters, maxUStencil_)
    , maxUBoundaryIterator_(flowField_, parameters, maxUStencil_)
    , globalBoundaryFactory_(parameters)
    , wallVelocityIterator_(globalBoundaryFactory_.getGlobalBoundaryVelocityIterator(flowField_))
    , wallFGHIterator_(globalBoundaryFactory_.getGlobalBoundaryFGHIterator(flowField_))
    , rhsStencil_(parameters)
    , rhsIterator_(flowField_, parameters, rhsStencil_)
    , velocityStencil_(parameters)
    , obstacleStencil_(parameters)
    , velocityIterator_(flowField_, parameters, velocityStencil_)
    , obstacleIterator_(flowField_, parameters, obstacleStencil_)
    , petscParallelManager_(parameters, flowField_)
// #ifdef BUILD_WITH_PETSC
     , solver_(std::make_unique<Solvers::PetscSolver>(flowField_, parameters))
// #else
//     , solver_(std::make_unique<Solvers::SORSolver>(flowField_, parameters)) 
// #endif
    , solverEigen_(std::make_unique<Solvers::EigenSolver>(flowField_, parameters))
{
    fghStencil_  = new Stencils::FGHStencil(parameters_);
    fghIterator_ = new FieldIterator<FlowField>(flowField_, parameters_, *fghStencil_);

    vtkStencil_  = new Stencils::VTKStencil(parameters_,
                                            flowField_.getCellsX(), flowField_.getCellsY(), flowField_.getCellsZ());
    vtkIterator_ = new FieldIterator<FlowField>(flowField_, parameters_, *vtkStencil_,
                                                parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset);
}

Simulation::~Simulation() {
    delete fghStencil_;
    delete fghIterator_;

    delete vtkStencil_;
    delete vtkIterator_;
}

void Simulation::initializeFlowField() {
    if (parameters_.simulation.scenario == "taylor-green") {
        // Currently, a particular initialization is only required for the taylor-green vortex.
        Stencils::InitTaylorGreenFlowFieldStencil stencil(parameters_);
        FieldIterator<FlowField> iterator(flowField_, parameters_, stencil);

        iterator.iterate();
    } else if (parameters_.simulation.scenario == "channel") {
        Stencils::BFStepInitStencil stencil(parameters_);
        FieldIterator<FlowField> iterator(flowField_, parameters_, stencil, 0, 1);

        iterator.iterate();
        wallVelocityIterator_.iterate();
    } else if (parameters_.simulation.scenario == "pressure-channel") {
        // Set pressure boundaries here for left wall
        const FLOAT value = parameters_.walls.scalarLeft;
        ScalarField& rhs = flowField_.getRHS();

        if (parameters_.geometry.dim == 2) {
            const int sizey = flowField_.getNy();
            for (int i = 0; i < sizey + 3; i++) {
                rhs.getScalar(0, i) = value;
            }
        } else {
            const int sizey = flowField_.getNy();
            const int sizez = flowField_.getNz();

            for (int i = 0; i < sizey + 3; i++) {
                for (int j = 0; j < sizez + 3; j++) {
                    rhs.getScalar(0, i, j) = value;
                }
            }
        }

        // Do same procedure for domain flagging as for regular channel
        Stencils::BFStepInitStencil stencil(parameters_);
        FieldIterator<FlowField> iterator(flowField_, parameters_, stencil, 0, 1);

        iterator.iterate();
    }

    solver_->reInitMatrix();
}

FLOAT Simulation::getDiffusiveTimestep_() {
    FLOAT factor = 1.0 / (parameters_.meshsize->getDxMin() * parameters_.meshsize->getDxMin()) +
            1.0 / (parameters_.meshsize->getDyMin() * parameters_.meshsize->getDyMin());

    if (parameters_.geometry.dim == 3) { // 3D
        factor += 1.0 / (parameters_.meshsize->getDzMin() * parameters_.meshsize->getDzMin());
    }

    return parameters_.flow.Re / (2 * factor);
}

void Simulation::setTimestep_() {
    ASSERTION(parameters_.geometry.dim == 2 || parameters_.geometry.dim == 3);
    FLOAT localMin, globalMin;

    // Determine maximum velocity
    maxUStencil_.reset();
    maxUFieldIterator_.iterate();
    maxUBoundaryIterator_.iterate();

    if (parameters_.geometry.dim == 2) { // 2D
        parameters_.timestep.dt = 1.0 / maxUStencil_.getMaxValues()[0];
    } else { // 3D
        parameters_.timestep.dt = 1.0 / maxUStencil_.getMaxValues()[2];
    }

    // Gets the diffusive timestep and uses that to get the local min
    const FLOAT diffusiveTimestep = getDiffusiveTimestep_();
    localMin = std::min(diffusiveTimestep,
                        std::min(parameters_.timestep.dt,
                                 std::min(1 / maxUStencil_.getMaxValues()[0], 1 / maxUStencil_.getMaxValues()[1])));

    // Here, we select the type of operation before compiling. This allows to use the correct
    // data type for MPI. Not a concern for small simulations, but useful if using heterogeneous
    // machines.

    globalMin = MY_FLOAT_MAX;
    MPI_Allreduce(&localMin, &globalMin, 1, MY_MPI_FLOAT, MPI_MIN, PETSC_COMM_WORLD);

    parameters_.timestep.dt = globalMin;
    parameters_.timestep.dt *= parameters_.timestep.tau;
}

void Simulation::solveTimestep(double& duration) {
    // Determine and set max timestep which is allowed in this simulation
    setTimestep_();

    fghIterator_->iterate(); // Compute FGH
    wallFGHIterator_.iterate(); // Set global boundary values
    rhsIterator_.iterate(); // Compute the right-hand side (RHS)

    std::chrono::time_point <std::chrono::high_resolution_clock> t0 = std::chrono::high_resolution_clock::now();

    // Solve for pressure
    solver_->solve(); // PetSC
    solverEigen_->solve(); // Eigen

    std::chrono::time_point <std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
    duration += std::chrono::duration<double>(t1 - t0).count();

    // Communicate pressure values
    petscParallelManager_.communicatePressure();

    // Compute velocity
    velocityIterator_.iterate();
    obstacleIterator_.iterate();

    // Communicate velocity values
    petscParallelManager_.communicateVelocity();

    // Iterate for velocities on the boundary
    wallVelocityIterator_.iterate();
}

void Simulation::plotVTK(int timestep) {
    vtkIterator_->iterate();

    vtkStencil_->write(timestep);
    vtkStencil_->clearValues(true);
}

} // namespace NSEOF
