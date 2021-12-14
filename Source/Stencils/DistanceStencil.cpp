#include "DistanceStencil.hpp"

namespace NSEOF::Stencils {

DistanceStencil::DistanceStencil(const Parameters& parameters, const int cellsX, const int cellsY, const int cellsZ)
    : FieldStencil<FlowField>(parameters)
    , cellsX_(cellsX), cellsY_(cellsY), cellsZ_(cellsZ)
    , stepXBound_(cellsX * parameters.bfStep.xRatio), stepYBound_(ceil(cellsY * parameters.bfStep.yRatio)) {}

/**
 * Calculates the distance to the nearest wall in the given direction
 *
 * Note: To limit if conditions we are only checking for u-velocities on each wall
 */
FLOAT DistanceStencil::calculateDistToNearestWallInGivenDir_(const FLOAT firstWallU, const FLOAT secondWallU,
                                                             const int idx, const FLOAT cellSize, const FLOAT sizeInOneDir) {
    const FLOAT leftDist = idx;
    const FLOAT rightDist = sizeInOneDir - idx;

    // check if a wall has u-velocity
    if (firstWallU == 0 && secondWallU == 0) {
        const int closestDist = idx <= sizeInOneDir / 2 ? leftDist : rightDist;
        return std::abs(closestDist * cellSize);
    } else if (firstWallU == 0 && secondWallU != 0) {
        return std::abs(leftDist * cellSize);
    } else if (firstWallU != 0 && secondWallU == 0) {
        return std::abs(rightDist * cellSize);
    }

    // distance is maximal if both walls are not actually walls!
    return std::numeric_limits<FLOAT>::infinity();
}

/**
 * Only goes into the following loops if there is a step
 *
 * Note: we check both loops in the same z coordinate as the original point
 *       so don't need to count it as there is no step in the Z-axis
 */
void DistanceStencil::calculateSteps_(FLOAT& distToWall, int i, int j, int k) {
    // Top boundary loop
    for (int x = 0; x < stepXBound_; x++) {
        FLOAT dx = (i - x) * parameters_.meshsize->getDx(i, j, k);
        FLOAT dy = (j - stepYBound_) * parameters_.meshsize->getDy(i, j, k);
        FLOAT stepDis = sqrt((dx * dx) + (dy * dy));

        distToWall = std::abs(std::min(distToWall, stepDis));
    }

    // Left boundary loop
    for (int y = 0; y < stepYBound_; y++) {
        FLOAT dx = (i - stepXBound_) * parameters_.meshsize->getDx(i, j, k);
        FLOAT dy = (j - y) * parameters_.meshsize->getDy(i, j, k);
        FLOAT stepDis = sqrt((dx * dx) + (dy * dy));

        distToWall = std::abs(std::min(distToWall, stepDis));
    }
}

void DistanceStencil::apply(FlowField& flowField, int i, int j, int k) {
    FLOAT& distToWall = flowField.getDistance().getScalar(i, j, k);

    if ((flowField.getFlags().getValue(i, j, k) & OBSTACLE_SELF) != 0) { // If it is not a fluid cell, the dist is zero.
        distToWall = 0;
    } else { // If it is a fluid cell, calculate the distance
        // check if left or right wall has u-velocity
        const FLOAT distX = calculateDistToNearestWallInGivenDir_(parameters_.walls.vectorLeft[0],
                                                                  parameters_.walls.vectorRight[0],
                                                                  i, parameters_.meshsize->getDx(i, j, k), cellsX_);

        // check if top or bottom wall has u-velocity
        const FLOAT distY = calculateDistToNearestWallInGivenDir_(parameters_.walls.vectorBottom[0],
                                                                  parameters_.walls.vectorTop[0],
                                                                  j, parameters_.meshsize->getDy(i, j, k), cellsY_);

        if (parameters_.geometry.dim == 2) { // 2D
            // Find the distance of cell to the nearest wall
            distToWall = std::abs(std::min(distX, distY));
        } else { // 3D
            // check if front or back wall has u-velocity
            const FLOAT distZ = calculateDistToNearestWallInGivenDir_(parameters_.walls.vectorFront[0],
                                                                      parameters_.walls.vectorBack[0],
                                                                      k, parameters_.meshsize->getDz(i, j, k), cellsZ_);

            // Find the distance of cell to the nearest wall
            distToWall = std::abs(std::min(std::min(distX, distY), distZ));
        }

        // Calculate steps if there are any
        calculateSteps_(distToWall, i, j, k);
    }
}

void DistanceStencil::apply(FlowField& flowField, int i, int j) {
    apply(flowField, i, j, 0);
}

} // namespace NSEOF::Stencils
