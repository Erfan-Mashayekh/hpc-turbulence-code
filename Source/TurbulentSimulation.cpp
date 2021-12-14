#include "TurbulentSimulation.hpp"

namespace NSEOF {

TurbulentSimulation::TurbulentSimulation(Parameters& parameters, FlowField& flowField)
    : Simulation(parameters, flowField)
    , minTimeStepStencil_(parameters)
    , minTimeStepIterator_(flowField, parameters, minTimeStepStencil_)
    , viscosityStencil_(parameters)
    , viscosityIterator_(flowField, parameters, viscosityStencil_)
    , distanceStencil_(parameters, flowField.getCellsX(), flowField.getCellsY(), flowField.getCellsZ())
    , distanceIterator_(flowField, parameters, distanceStencil_)
    , turbulentPetscParallelManager_(parameters)
    , viscosityBufferFillStencil_(parameters)
    , viscosityBufferReadStencil_(parameters)
    , viscosityBufferFillIterator_(flowField, parameters, viscosityBufferFillStencil_,
                                   parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
    , viscosityBufferReadIterator_(flowField, parameters, viscosityBufferReadStencil_,
                                   parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
{
    fghStencil_  = new Stencils::TurbulentFGHStencil(parameters_);
    fghIterator_ = new FieldIterator<FlowField>(flowField_, parameters_, *fghStencil_);

    vtkStencil_  = new Stencils::TurbulentVTKStencil(parameters_,
                                                     flowField_.getCellsX(), flowField_.getCellsY(), flowField_.getCellsZ());
    vtkIterator_ = new FieldIterator<FlowField>(flowField_, parameters_, *vtkStencil_);
}

FLOAT TurbulentSimulation::getDiffusiveTimestep_() {
    // reset diffusive timestep stencil and determine min diffusive timestep
    minTimeStepStencil_.reset();
    minTimeStepIterator_.iterate();

    // return the minimum diffusive timestep
    return minTimeStepStencil_.getDiffusiveTimeStep();
}

void TurbulentSimulation::solveTimestep() {
    Simulation::solveTimestep();

    viscosityIterator_.iterate(); // Compute eddy viscosities
    distanceIterator_.iterate(); // Compute distances to the closest walls

    // communicate viscosity values
    viscosityBufferFillIterator_.iterate();
    turbulentPetscParallelManager_.communicateViscosity(viscosityBufferFillStencil_, viscosityBufferReadStencil_);
    viscosityBufferReadIterator_.iterate();
}

} // namespace NSEOF
