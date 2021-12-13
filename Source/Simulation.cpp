#include "Simulation.hpp"

#include "Stencils/VTKStencil.hpp"
#include "Stencils/VTKTurbulenceStencil.hpp"

#include "Solvers/SORSolver.hpp"
#include "Solvers/PetscSolver.hpp"

#include <limits>

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
    , fghStencil_(parameters)
    , fghIterator_(flowField_, parameters, fghStencil_)
    , rhsStencil_(parameters)
    , rhsIterator_(flowField_, parameters, rhsStencil_)
    , velocityStencil_(parameters)
    , obstacleStencil_(parameters)
    , velocityIterator_(flowField_, parameters, velocityStencil_)
    , obstacleIterator_(flowField_, parameters, obstacleStencil_)
    , viscosityStencil_(parameters)
    , viscosityIterator_(flowField_, parameters, viscosityStencil_)
    , minTimeStepStencil_(parameters)
    , minTimeStepIterator_(flowField_, parameters, minTimeStepStencil_)
    , petscParallelManager_(parameters)
    , pressureBufferFillStencil_(parameters)
    , pressureBufferReadStencil_(parameters)
    , velocityBufferFillStencil_(parameters)
    , velocityBufferReadStencil_(parameters)
    , pressureBufferFillIterator_(flowField_, parameters, pressureBufferFillStencil_,
                                  parameters_.vtk.whiteRegionLowOffset, parameters_.vtk.whiteRegionHighOffset)
    , pressureBufferReadIterator_(flowField_, parameters, pressureBufferReadStencil_,
                                  parameters_.vtk.whiteRegionLowOffset, parameters_.vtk.whiteRegionHighOffset)
    , velocityBufferFillIterator_(flowField_, parameters, velocityBufferFillStencil_,
                                  parameters_.vtk.whiteRegionLowOffset, parameters_.vtk.whiteRegionHighOffset)
    , velocityBufferReadIterator_(flowField_, parameters, velocityBufferReadStencil_,
                                  parameters_.vtk.whiteRegionLowOffset, parameters_.vtk.whiteRegionHighOffset)
#ifdef BUILD_WITH_PETSC
    , solver_(std::make_unique<Solvers::PetscSolver>(flowField_, parameters))
#else
    , solver_(std::make_unique<Solvers::SORSolver>(flowField_, parameters))
#endif
    {}

void Simulation::calculateDistancesToNearestWalls() {
    // Number of cells in each direction plus the ghost cells
    const int sizeX = flowField_.getNx() + 3;
    const int sizeY = flowField_.getNy() + 3;
    const int sizeZ = parameters_.geometry.dim == 3 ? flowField_.getNz() + 3 : 1;

    // Distance to each axis
    FLOAT distX, distY, distZ = std::numeric_limits<FLOAT>::infinity();

    /**
     * In case there is a step geometry,
     * number of cells in each direction that make up the step
     * parameters.bfStep.xRatio and parameters.bfStep.yRatio are -1 if no step
     */
    int stepXBound = ceil(sizeX * parameters_.bfStep.xRatio);
    int stepYBound = ceil(sizeY * parameters_.bfStep.yRatio);

    // Field to store distance to wall
    ScalarField& distance_to_wall = flowField_.getDistance();

    for (int k = 0; k < sizeZ; k++) {
        for (int j = 0; j < sizeY; j++) {
            for (int i = 0; i < sizeX; i++) {
                int obstacle = flowField_.getFlags().getValue(i, j, k);

                if ((obstacle & OBSTACLE_SELF) != 0) { // If it is not a fluid cell, the dist is zero.
                    distance_to_wall.getScalar(i, j, k) = 0;
                } else { // If it is a fluid cell, calculate the distance
                
                	//Note: To limit if conditions we are only checking for u-velocities on each wall
                	//check if left or right wall has u-velocity 
                	if ((parameters_.walls.vectorLeft[0] == 0) && (parameters_.walls.vectorRight[0] == 0)) {
                    	distX = std::abs((i <= sizeX / 2 ? i : sizeX - i) * parameters_.meshsize->getDx(i, j, k));
                    }
                    else if ((parameters_.walls.vectorLeft[0] == 0) && (parameters_.walls.vectorRight[0] != 0)) {
                    	distX = std::abs(i * parameters_.meshsize->getDx(i, j, k));	
                    }
                    else if ((parameters_.walls.vectorLeft[0] != 0) && (parameters_.walls.vectorRight[0] == 0)) {
                    	distX = std::abs((sizeX - i) * parameters_.meshsize->getDx(i, j, k));	
                    }
                    else {
                    	distX = std::numeric_limits<FLOAT>::infinity();
                    }
                    
                    //check if top or bottom wall has u-velocity 
                	if ((parameters_.walls.vectorBottom[0] == 0) && (parameters_.walls.vectorTop[0] == 0)) {
                    	distY = std::abs((j <= sizeY / 2 ? j : sizeY - j) * parameters_.meshsize->getDy(i, j, k));
                    }
                    else if ((parameters_.walls.vectorBottom[0] == 0) && (parameters_.walls.vectorTop[0] != 0)) {
                    	distY = std::abs(j * parameters_.meshsize->getDy(i, j, k));	
                    }
                    else if ((parameters_.walls.vectorBottom[0] != 0) && (parameters_.walls.vectorTop[0] == 0)) {
                    	distY = std::abs((sizeY - j) * parameters_.meshsize->getDy(i, j, k));
                    }
                    else {
                    	distY = std::numeric_limits<FLOAT>::infinity();
                    }

                    if (parameters_.geometry.dim == 3) { // 3D
                        //check if front or back wall has u-velocity 
		            	if ((parameters_.walls.vectorBack[0] == 0) && (parameters_.walls.vectorFront[0] == 0)) {
		                	distZ = std::abs((k <= sizeZ / 2 ? k : sizeZ - k) * parameters_.meshsize->getDz(i, j, k));
		                }
		                else if ((parameters_.walls.vectorBack[0] == 0) && (parameters_.walls.vectorFront[0] != 0)) {
		                	distZ = std::abs(k * parameters_.meshsize->getDz(i, j, k));	
		                }
		                else if ((parameters_.walls.vectorBottom[0] != 0) && (parameters_.walls.vectorTop[0] == 0)) {
		                	distZ = std::abs((sizeZ - k) * parameters_.meshsize->getDz(i, j, k));
		                }
		                else{
		                	distZ = std::numeric_limits<FLOAT>::infinity();
		                }
                    }

                    // Find the distance of cell to the nearest wall
                    distance_to_wall.getScalar(i, j, k) = std::abs(std::min(std::min(distX, distY), distZ));

                    /**
                     * Only goes into the following loops if there is a step
                     *
                     * Note: we check both loops in the same z coordinate as the original point
                     *       so don't need to count it as there is no step in the Z-axis
                     */

                    // top boundary loop
                    for (int x = 0; x < stepXBound; x++) {
                        FLOAT dx = (i - x) * parameters_.meshsize->getDx(i, j, k);
                        FLOAT dy = (j - stepYBound) * parameters_.meshsize->getDy(i, j, k);
                        FLOAT stepDis = sqrt((dx * dx) + (dy * dy));

                        distance_to_wall.getScalar(i, j, k) = std::abs(std::min(distance_to_wall.getScalar(i, j, k), stepDis));
                    }

                    // left boundary loop
                    for (int y = 0; y < stepYBound; y++) {
                        FLOAT dx = (i - stepXBound) * parameters_.meshsize->getDx(i, j, k);
                        FLOAT dy = (j - y) * parameters_.meshsize->getDy(i, j, k);
                        FLOAT stepDis = sqrt((dx * dx) + (dy * dy));

                        distance_to_wall.getScalar(i, j, k) = std::abs(std::min(distance_to_wall.getScalar(i, j, k), stepDis));
                    }
                }
                std::cout << "distance to wall " << distance_to_wall.getScalar(i, j, k) << "	dx " << distX <<"	dy " << distY << "	dz " << distZ << "	i " << i << "	j " << j << "	k " << k << std::endl;
            }
        }
    }
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

    // TODO WS2: solve and store the nearest distance calculation
    Simulation::calculateDistancesToNearestWalls();
}

void Simulation::solveTimestep() {
    // Determine and set max. timestep which is allowed in this simulation
    setTimeStep();

    viscosityIterator_.iterate(); // Compute eddy viscosity
    fghIterator_.iterate(); // Compute FGH
    wallFGHIterator_.iterate(); // Set global boundary values
    rhsIterator_.iterate(); // Compute the right-hand side (RHS)

    // Solve for pressure
    solver_->solve();

    // TODO WS2: communicate pressure values
    pressureBufferFillIterator_.iterate();
    petscParallelManager_.communicatePressure(pressureBufferFillStencil_, pressureBufferReadStencil_);
    pressureBufferReadIterator_.iterate();

    // Compute velocity
    velocityIterator_.iterate();
    obstacleIterator_.iterate();

    // TODO WS2: communicate velocity values
    velocityBufferFillIterator_.iterate();
    petscParallelManager_.communicateVelocity(velocityBufferFillStencil_, velocityBufferReadStencil_);
    velocityBufferReadIterator_.iterate();

    // Iterate for velocities on the boundary
    wallVelocityIterator_.iterate();
}

void Simulation::plotVTK(int timeStep) {
    // TODO WS1: create VTKStencil and respective iterator; iterate stencil
    //           over flowField_ and write flow field information to VTK file.
    Stencils::VTKStencil* vtkStencil_;

    if (parameters_.turbulence.on == 1) { // Turbulence Model
        vtkStencil_ = new Stencils::VTKTurbulenceStencil(
                parameters_, flowField_.getCellsX(), flowField_.getCellsY(), flowField_.getCellsZ());
    } else { // Laminar Model
        vtkStencil_ = new Stencils::VTKStencil(
                parameters_, flowField_.getCellsX(), flowField_.getCellsY(), flowField_.getCellsZ());
    }

    FieldIterator<FlowField> vtkIterator_(flowField_, parameters_, *vtkStencil_,
                                          parameters_.vtk.whiteRegionLowOffset, parameters_.vtk.whiteRegionHighOffset);

    vtkIterator_.iterate();
    vtkStencil_->write(timeStep);

    delete vtkStencil_;
}

void Simulation::setTimeStep() {
    ASSERTION(parameters_.geometry.dim == 2 || parameters_.geometry.dim == 3);

	FLOAT localMin, globalMin;

	// Determine maximum velocity
	maxUStencil_.reset();
	maxUFieldIterator_.iterate();
	maxUBoundaryIterator_.iterate();

    if (parameters_.geometry.dim == 3) { // 3D
        parameters_.timestep.dt = 1.0 / maxUStencil_.getMaxValues()[2];
    } else { // 2D
        parameters_.timestep.dt = 1.0 / maxUStencil_.getMaxValues()[0];
    }

	if (parameters_.turbulence.on == 1) {
		// reset diffusive timestep stencil and determine min diffusive timestep
		minTimeStepStencil_.reset();
		minTimeStepIterator_.iterate();

		// store minimum diffusive timestep
		FLOAT diffusiveTimeStep = minTimeStepStencil_.getDiffusiveTimeStep();

		localMin = std::min(diffusiveTimeStep, std::min(parameters_.timestep.dt, std::min(1 / maxUStencil_.getMaxValues()[0], 1 / maxUStencil_.getMaxValues()[1])));
	} else {
		FLOAT factor = 1.0 / (parameters_.meshsize->getDxMin() * parameters_.meshsize->getDxMin()) + 1.0 / (parameters_.meshsize->getDyMin() * parameters_.meshsize->getDyMin());

		if (parameters_.geometry.dim == 3) { // 3D
		    factor += 1.0 / (parameters_.meshsize->getDzMin() * parameters_.meshsize->getDzMin());
		}

		localMin = std::min(parameters_.flow.Re / (2 * factor), std::min(parameters_.timestep.dt, std::min(1 / maxUStencil_.getMaxValues()[0], 1 / maxUStencil_.getMaxValues()[1])));
    }

    // Here, we select the type of operation before compiling. This allows to use the correct
	// data type for MPI. Not a concern for small simulations, but useful if using heterogeneous
	// machines.

    globalMin = MY_FLOAT_MAX;
    MPI_Allreduce(&localMin, &globalMin, 1, MY_MPI_FLOAT, MPI_MIN, PETSC_COMM_WORLD);

    parameters_.timestep.dt = globalMin;
    parameters_.timestep.dt *= parameters_.timestep.tau;
}

} // namespace NSEOF
