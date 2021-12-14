#include "TurbulentFGHStencil.hpp"

namespace NSEOF::Stencils {

TurbulentFGHStencil::TurbulentFGHStencil(const Parameters& parameters)
    : FGHStencil(parameters) {}

void TurbulentFGHStencil::computeValues_(FlowField& flowField, int i, int j) {
    loadLocalViscosity2D(parameters_, flowField, localViscosity_, i, j);

    FLOAT* values = flowField.getFGH().getVector(i, j);

    values[0] = computeF2DT(getLocalVelocity_(), getLocalMeshsize_(),
                            localViscosity_, parameters_, parameters_.timestep.dt);
    values[1] = computeG2DT(getLocalVelocity_(), getLocalMeshsize_(),
                            localViscosity_, parameters_, parameters_.timestep.dt);
}

void TurbulentFGHStencil::computeValues_(FlowField& flowField, int i, int j, int k, const int obstacle) {
    loadLocalViscosity3D(parameters_, flowField, localViscosity_, i, j, k);

    FLOAT* values = flowField.getFGH().getVector(i, j, k);

    if ((obstacle & OBSTACLE_RIGHT) == 0) {
        values[0] = computeF3DT(getLocalVelocity_(), getLocalMeshsize_(),
                                localViscosity_, parameters_, parameters_.timestep.dt);
    }

    if ((obstacle & OBSTACLE_TOP) == 0) {
        values[1] = computeG3DT(getLocalVelocity_(), getLocalMeshsize_(),
                                localViscosity_, parameters_, parameters_.timestep.dt);
    }

    if ((obstacle & OBSTACLE_BACK) == 0) {
        values[2] = computeH3DT(getLocalVelocity_(), getLocalMeshsize_(),
                                localViscosity_, parameters_, parameters_.timestep.dt);
    }
}

} // namespace NSEOF::Stencils