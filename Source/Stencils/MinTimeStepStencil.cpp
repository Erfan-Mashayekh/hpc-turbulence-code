#include "MinTimeStepStencil.hpp"

namespace NSEOF::Stencils {

MinTimeStepStencil::MinTimeStepStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters) {
        reset();
}

void MinTimeStepStencil::apply(FlowField& flowField, int i, int j, int k) {
    const FLOAT dx = FieldStencil<FlowField>::parameters_.meshsize->getDx(i, j, k);
    const FLOAT dy = FieldStencil<FlowField>::parameters_.meshsize->getDy(i, j, k);
    FLOAT dz = 0;

    if (parameters_.geometry.dim == 3) { // 3D
        dz = FieldStencil<FlowField>::parameters_.meshsize->getDz(i, j, k);
    }

    const FLOAT eddyViscosity = flowField.getEddyViscosity().getScalar(i, j, k);

    // Eddy viscosity plus kinematic viscosity
    FLOAT totalViscosity = 1 / FieldStencil<FlowField>::parameters_.flow.Re + eddyViscosity;

    // Reynolds number new
    FLOAT reynoldsNew = 1 / totalViscosity;
    FLOAT factor1 = 1.0 / (dx * dx) + 1.0 / (dy * dy);
    FLOAT factor2 = 1.0 / (dx * dx) + 1.0 / (dy * dy) + 1 / (dx * dy);

    if (parameters_.geometry.dim == 3) { // 3D
        factor1 += 1.0 / (dz * dz);
        factor2 += 1 / (dx * dz) + 1 / (dy * dz);
    }

    // Timestep minimum in current cell
    FLOAT cellTimeStep = std::min((reynoldsNew / (2 * factor1)), reynoldsNew / (2 * factor2));

    if (std::abs(cellTimeStep) < diffusiveTimeStep_) {
        diffusiveTimeStep_ = cellTimeStep;
    }
}

void MinTimeStepStencil::apply(FlowField& flowField, int i, int j) {
    apply(flowField, i, j, 0);
}

void MinTimeStepStencil::reset() {
	// Initially set diffusive timestep to the given one from the user
	// Hard coded, because parameters_.timestep.dt is changed during the simulation
    diffusiveTimeStep_ = 1.; // FieldStencil<FlowField>::parameters_.timestep.dt;
}

FLOAT MinTimeStepStencil::getDiffusiveTimeStep() const {
    return diffusiveTimeStep_;
}

} // namespace NSEOF::Stencils
