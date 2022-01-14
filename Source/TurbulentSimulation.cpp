#include "TurbulentSimulation.hpp"

namespace NSEOF {

TurbulentSimulation::TurbulentSimulation(Parameters& parameters, FlowField& flowField)
    : Simulation(parameters, flowField)
    , minTimeStepStencil_(parameters)
    , minTimeStepIterator_(flowField, parameters, minTimeStepStencil_)
    , distanceStencil_(parameters, parameters.geometry.sizeX + 3, parameters.geometry.sizeY + 3, parameters.geometry.sizeZ + 3)
    , distanceIterator_(flowField, parameters, distanceStencil_)
    , viscosityStencil_(parameters)
    , viscosityIterator_(flowField, parameters, viscosityStencil_)
    , turbulentPetscParallelManager_(parameters, flowField)
{
    fghStencil_  = new Stencils::TurbulentFGHStencil(parameters_);
    fghIterator_ = new FieldIterator<FlowField>(flowField_, parameters_, *fghStencil_);

    vtkStencil_  = new Stencils::TurbulentVTKStencil(parameters_,
                                                     flowField_.getCellsX(), flowField_.getCellsY(), flowField_.getCellsZ());
    vtkIterator_ = new FieldIterator<FlowField>(flowField_, parameters_, *vtkStencil_,
                                                parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset);
}

void TurbulentSimulation::initializeFlowField() {
    Simulation::initializeFlowField();

    // Compute distances to the closest walls
    distanceIterator_.iterate();
}

FLOAT TurbulentSimulation::getDiffusiveTimestep_() {
    // reset diffusive timestep stencil and determine min diffusive timestep
    minTimeStepStencil_.reset();
    minTimeStepIterator_.iterate();

    // return the minimum diffusive timestep
    return minTimeStepStencil_.getDiffusiveTimeStep();
}

void TurbulentSimulation::solveTimestep(double& duration) {
    Simulation::solveTimestep(duration);

    // If the turbulence viscosity flag is set to zero, do not iterate for viscosity!
    if (parameters_.turbulence.turbViscosity == 0) {
        return;
    }

    // Communicate diagonal velocity values before as they are also needed for viscosity computations
    turbulentPetscParallelManager_.communicateDiagonalVelocity();

    // Compute eddy viscosities
    viscosityIterator_.iterate();

    // Communicate viscosity values
    turbulentPetscParallelManager_.communicateViscosity();
}

} // namespace NSEOF
