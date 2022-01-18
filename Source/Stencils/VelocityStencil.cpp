#include "VelocityStencil.hpp"

namespace NSEOF::Stencils {

VelocityStencil::VelocityStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters) {}

void VelocityStencil::apply(FlowField& flowField, int i, int j, int k) {
    const FLOAT dt = parameters_.timestep.dt;
    const int obstacle = flowField.getFlags().getValue(i, j, k);
    VectorField& velocity = flowField.getVelocity();

    if ((obstacle & OBSTACLE_SELF) == 0) { // If this is a fluid cell
        if ((obstacle & OBSTACLE_RIGHT) == 0) {
            /**
             * We require a spatial finite difference expression for the pressure gradient, evaluated
             * at the location of the u-component. We therefore compute the distance of neighbouring
             * pressure values (dx) and use this as sort-of central difference expression. This will
             * yield second-order accuracy for uniform meshsizes.
             *
             * Note: We only set one direction per cell. The neighbor at the left is responsible for the other side.
             */
            const FLOAT dx = 0.5 * (parameters_.meshsize->getDx(i, j, k) + parameters_.meshsize->getDx(i + 1, j, k));
            velocity.getVector(i, j, k)[0] = flowField.getFGH().getVector(i, j, k)[0] - dt / dx *
                    (flowField.getPressure().getScalar(i + 1, j, k) - flowField.getPressure().getScalar(i, j, k));
        } else {
            velocity.getVector(i, j, k)[0] = 0.0;
        }

        if ((obstacle & OBSTACLE_TOP) == 0) {
            const FLOAT dy = 0.5 * (parameters_.meshsize->getDy(i, j, k) + parameters_.meshsize->getDy(i, j + 1, k));
            velocity.getVector(i, j, k)[1] = flowField.getFGH().getVector(i, j, k)[1] - dt / dy *
                    (flowField.getPressure().getScalar(i, j + 1, k) - flowField.getPressure().getScalar(i, j, k));
        } else {
            velocity.getVector(i, j, k)[1] = 0.0;
        }

        if (parameters_.geometry.dim == 3 && (obstacle & OBSTACLE_BACK) == 0) {
            const FLOAT dz = 0.5 * (parameters_.meshsize->getDz(i, j, k) + parameters_.meshsize->getDz(i, j, k + 1));
            velocity.getVector(i, j, k)[2] = flowField.getFGH().getVector(i, j, k)[2] - dt / dz *
                    (flowField.getPressure().getScalar(i, j, k + 1) - flowField.getPressure().getScalar(i, j, k));
        } else {
            velocity.getVector(i, j, k)[2] = 0.0;
        }
    }
}

void VelocityStencil::apply(FlowField& flowField, int i, int j) {
    apply(flowField, i, j, 0);
}

} // namespace NSEOF::Stencils
