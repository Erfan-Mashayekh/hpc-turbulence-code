#include "BFInputStencils.hpp"

namespace NSEOF {
namespace Stencils {

FLOAT computeVelocity3D(int i, int j, int k, FLOAT stepSize, const Parameters& parameters) {
    const FLOAT posY = parameters.meshsize->getPosY(i, j, k);
    const FLOAT posZ = parameters.meshsize->getPosZ(i, j, k);
    const FLOAT dy   = parameters.meshsize->getDy(i, j, k);
    const FLOAT dz   = parameters.meshsize->getDz(i, j, k);

    if (posY + 0.5 * dy >= stepSize) {
        // Get the size of the inlet in Y. A 3 is subtracted because of the boundary cells.
        const FLOAT inletYSize = parameters.geometry.lengthY - stepSize;
        const FLOAT inletZSize = parameters.geometry.lengthZ;

        const FLOAT y = posY + 0.5 * dy - stepSize;
        const FLOAT z = posZ + 0.5 * dz;

        return 36.0 * parameters.walls.vectorLeft[0] / (inletZSize * inletZSize * inletYSize * inletYSize) *  y * (y - inletYSize) * z * (z - inletZSize);
    } else {
        return 0.0;
    }
}

FLOAT computeVelocity2D(int i, int j, FLOAT stepSize, const Parameters& parameters){
    const FLOAT posY = parameters.meshsize->getPosY(i, j);
    const FLOAT dy   = parameters.meshsize->getDy(i, j);

    if (posY + 0.5 * dy >= stepSize) {
        // Get the size of the inlet in Y. A 3 is subtracted because of the boundary cells.
        const FLOAT inletYSize = parameters.geometry.lengthY - stepSize;

        const FLOAT y = posY + 0.5 * dy - stepSize;

        // For turbulence, please use: return parameters.walls.vectorLeft[0];
        return 6.0 * parameters.walls.vectorLeft[0] / (inletYSize * inletYSize) * y * (inletYSize - y);
    } else {
        return 0.0;
    }
}

BFInputVelocityStencil::BFInputVelocityStencil(const Parameters& parameters)
    : BoundaryStencil<FlowField>(parameters)
    // Here, the obstacle size is set to zero if it was set as negative at the configuration.
    , stepSize_(parameters.bfStep.yRatio > 0.0 ? parameters.bfStep.yRatio * parameters.geometry.lengthY : 0.0) {

    if (parameters_.geometry.dim == 2) {
        FLOAT posY   = parameters_.meshsize->getPosY(0, 0);
        FLOAT dy     = parameters_.meshsize->getDy(0, 0);
        FLOAT nextDy = parameters_.meshsize->getDy(0, 1);

        for (int j = 0; j < parameters_.geometry.sizeY - 1; ++j) {
            posY   = parameters_.meshsize->getPosY(0, j);
            dy     = parameters_.meshsize->getDy(0, j);
            nextDy = parameters_.meshsize->getDy(0, j + 1);

            // Check if stepSize is in this cell
            if (posY + 0.5 * dy < stepSize_ && stepSize_ <= posY + dy + 0.5 * nextDy) {
                stepSize_ = posY + dy;
                break;
            }
        }
    } else if(parameters_.geometry.dim == 3) {
        FLOAT posY   = parameters_.meshsize->getPosY(0, 0, 0);
        FLOAT dy     = parameters_.meshsize->getDy(0, 0, 0);
        FLOAT nextDy = parameters_.meshsize->getDy(0, 1, 0);

        for (int j = 0; j < parameters_.geometry.sizeY - 1; ++j) {
            posY   = parameters_.meshsize->getPosY(0, j, 0);
            dy     = parameters_.meshsize->getDy(0, j, 0);
            nextDy = parameters_.meshsize->getDy(0, j + 1, 0);

            if (posY + 0.5 * dy < stepSize_ && stepSize_ <= posY + dy + 0.5 * nextDy) {
                stepSize_ = posY + dy;
                break;
            }
        }
    }
}

void BFInputVelocityStencil::applyLeftWall(FlowField& flowField, int i, int j) {
    flowField.getVelocity().getVector(i, j)[0] = computeVelocity2D(i, j, stepSize_, parameters_);
    flowField.getVelocity().getVector(i, j)[1] = -flowField.getVelocity().getVector(i + 1,j)[1];
}

// Most of the functions are empty, and they shouldn't be called, assuming that the input is always located at the left.
void BFInputVelocityStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
void BFInputVelocityStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
void BFInputVelocityStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}

void BFInputVelocityStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
    flowField.getVelocity().getVector(i,j,k)[0] = computeVelocity3D(i, j, k, stepSize_, parameters_);
    flowField.getVelocity().getVector(i,j,k)[1] = -flowField.getVelocity().getVector(i + 1, j, k)[1];
    flowField.getVelocity().getVector(i,j,k)[2] = -flowField.getVelocity().getVector(i + 1, j, k)[2];
}

void BFInputVelocityStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
void BFInputVelocityStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
void BFInputVelocityStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
void BFInputVelocityStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
void BFInputVelocityStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}

BFInputFGHStencil::BFInputFGHStencil(const Parameters& parameters)
    : BoundaryStencil<FlowField>(parameters)
    , stepSize_(parameters.bfStep.yRatio > 0.0 ? parameters.bfStep.yRatio * parameters.geometry.lengthY : 0.0) {}

void BFInputFGHStencil::applyLeftWall(FlowField & flowField, int i, int j) {
    flowField.getFGH().getVector(i,j)[0] = computeVelocity2D(i, j, stepSize_, parameters_);
}

void BFInputFGHStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
void BFInputFGHStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
void BFInputFGHStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}

void BFInputFGHStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
    flowField.getFGH().getVector(i,j,k)[0] = computeVelocity3D(i, j, k, stepSize_, parameters_);
}

void BFInputFGHStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
void BFInputFGHStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
void BFInputFGHStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
void BFInputFGHStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
void BFInputFGHStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}

} // namespace Stencils
} // namespace NSEOF
