#include "MinTimeStepStencil.hpp"

#include <algorithm>
#include <math.h>

namespace NSEOF {
namespace Stencils {

MinTimeStepStencil::MinTimeStepStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters)
    {
    reset();
	}

void MinTimeStepStencil::apply(FlowField& flowField, int i, int j) {
    cellMinValue(flowField, i, j);
}

void MinTimeStepStencil::apply(FlowField& flowField, int i, int j, int k) {
    cellMinValue(flowField, i, j, k);
}


void MinTimeStepStencil::cellMinValue(FlowField& flowField, int i, int j) {

    const FLOAT dx = FieldStencil<FlowField>::parameters_.meshsize->getDx(i, j);
    const FLOAT dy = FieldStencil<FlowField>::parameters_.meshsize->getDy(i, j);
    const FLOAT eddyViscosity = flowField.getEddyViscosity().getScalar(i, j);
    
    //eddy viscosity plus kinematic visocisty
    FLOAT totalViscosity = 1/FieldStencil<FlowField>::parameters_.flow.Re + eddyViscosity;
    //Reynolds number new
    FLOAT ReynoldsNew = 1/totalViscosity;
    FLOAT factor = 1.0 / (dx * dx) + 1.0 / (dy * dy);
    FLOAT factor2 = 1.0 / (dx * dx) + 1.0 / (dy * dy) + 1/ ( dx * dy);
    //time step minimum in current cell:
    FLOAT cellTimeStep = std::min((ReynoldsNew/(2*factor)), ReynoldsNew/(2*factor2));
    
    
    if (cellTimeStep < diffusiveTimeStep_) {
        diffusiveTimeStep_ = cellTimeStep;
    }
}

void MinTimeStepStencil::cellMinValue(FlowField& flowField, int i, int j, int k) {

    const FLOAT dx = FieldStencil<FlowField>::parameters_.meshsize->getDx(i, j, k);
    const FLOAT dy = FieldStencil<FlowField>::parameters_.meshsize->getDy(i, j, k);
    const FLOAT dz = FieldStencil<FlowField>::parameters_.meshsize->getDz(i, j, k);
    const FLOAT eddyViscosity = flowField.getEddyViscosity().getScalar(i, j, k);
    
    //eddy viscosity plus kinematic visocisty
    FLOAT totalViscosity = 1/FieldStencil<FlowField>::parameters_.flow.Re + eddyViscosity;
    //Reynolds number new
    FLOAT ReynoldsNew = 1/totalViscosity;
    FLOAT factor = 1.0 / (dx * dx) + 1.0 / (dy * dy) + 1.0 / (dz * dz);
    FLOAT factor2 = 1.0 / (dx * dx) + 1.0 / (dy * dy) + 1/ ( dx * dy) + 1/(dx * dz) + 1/(dy * dz);
    //time step minimum in current cell:
    FLOAT cellTimeStep = std::min((ReynoldsNew/(2*factor)), ReynoldsNew/(2*factor2));
    
    if (std::abs(cellTimeStep) < diffusiveTimeStep_) {
        diffusiveTimeStep_ = cellTimeStep;
    }
}

void MinTimeStepStencil::reset() {
	//initially set diffusive time step to the given one from the user
	//hard coded, because parameters_.timestep.dt is changed during the simulation
    diffusiveTimeStep_ = 1.; //FieldStencil<FlowField>::parameters_.timestep.dt;
}

FLOAT MinTimeStepStencil::getDiffusiveTimeStep() const {
    return diffusiveTimeStep_;
}

} // namespace Stencils
} // namespace NSEOF
