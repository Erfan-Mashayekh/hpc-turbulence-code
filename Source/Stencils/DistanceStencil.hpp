#ifndef __STENCILS_DISTANCE_STENCIL_HPP__
#define __STENCILS_DISTANCE_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

#include <limits>

namespace NSEOF::Stencils {

/**
 * Stencil to compute the distances to the nearest walls
 */
class DistanceStencil : public FieldStencil<FlowField> {
private:
    const int cellsX_, cellsY_, cellsZ_;

    /**
     * Walls in each direction
     *
     * Cavity: There is always a wall on the global boundaries
     * Channel: There is always not a left and right walls on the global boundaries
     */
    const bool leftWall_;
    const bool rightWall_;
    const bool bottomWall_;
    const bool topWall_;
    const bool frontWall_;
    const bool backWall_;

    /**
     * In case there is a step geometry,
     * number of cells in each direction that make up the step
     * parameters.bfStep.xRatio and parameters.bfStep.yRatio are -1 if no step
     */
    const int stepXBound_, stepYBound_;

    FLOAT calculateDistToNearestWallInGivenDir_(bool, bool, int, FLOAT, FLOAT);
    void calculateSteps_(FLOAT&, int, int, int);

public:
    DistanceStencil(const Parameters& parameters, int, int, int);

    void apply(FlowField & flowField, int i, int j) override;
    void apply(FlowField & flowField, int i, int j, int k) override;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_DISTANCE_STENCIL_HPP__
