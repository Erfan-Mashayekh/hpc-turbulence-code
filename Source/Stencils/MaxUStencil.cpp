#include "MaxUStencil.hpp"

#include <algorithm>
#include <math.h>

namespace NSEOF {
namespace Stencils {

MaxUStencil::MaxUStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters)
    , BoundaryStencil<FlowField>(parameters) {

    reset();
}

void MaxUStencil::apply(FlowField& flowField, int i, int j) {
    cellMaxValue(flowField, i, j);
}

void MaxUStencil::apply(FlowField& flowField, int i, int j, int k) {
    cellMaxValue(flowField, i, j, k);
}

void MaxUStencil::applyLeftWall(FlowField& flowField, int i, int j) {
    cellMaxValue(flowField, i, j);
}

void MaxUStencil::applyRightWall(FlowField& flowField, int i, int j) {
    cellMaxValue(flowField, i, j);
}

void MaxUStencil::applyBottomWall(FlowField& flowField, int i, int j) {
    cellMaxValue(flowField, i, j);
}

void MaxUStencil::applyTopWall(FlowField& flowField, int i, int j) {
    cellMaxValue(flowField, i, j);
}

void MaxUStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
    cellMaxValue(flowField, i, j, k);
}

void MaxUStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
    cellMaxValue(flowField, i, j, k);
}

void MaxUStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
    cellMaxValue(flowField, i, j, k);
}

void MaxUStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
    cellMaxValue(flowField, i, j, k);
}

void MaxUStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
    cellMaxValue(flowField, i, j, k);
}

void MaxUStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
    cellMaxValue(flowField, i, j, k);
}

void MaxUStencil::cellMaxValue(FlowField& flowField, int i, int j) {
    FLOAT* velocity = flowField.getVelocity().getVector(i, j);
    const FLOAT dx = FieldStencil<FlowField>::parameters_.meshsize->getDx(i, j);
    const FLOAT dy = FieldStencil<FlowField>::parameters_.meshsize->getDy(i, j);
    if (fabs(velocity[0]) / dx > maxValues_[0]) {
        maxValues_[0] = fabs(velocity[0]) / dx;
    }
    if (fabs(velocity[1]) / dy > maxValues_[1]) {
        maxValues_[1] = fabs(velocity[1]) / dy;
    }
}

void MaxUStencil::cellMaxValue(FlowField& flowField, int i, int j, int k) {
    FLOAT* velocity = flowField.getVelocity().getVector(i, j, k);
    const FLOAT dx = FieldStencil<FlowField>::parameters_.meshsize->getDx(i, j, k);
    const FLOAT dy = FieldStencil<FlowField>::parameters_.meshsize->getDy(i, j, k);
    const FLOAT dz = FieldStencil<FlowField>::parameters_.meshsize->getDz(i, j, k);
    if (fabs(velocity[0]) / dx > maxValues_[0]) {
        maxValues_[0] = fabs(velocity[0]) / dx;
    }
    if (fabs(velocity[1]) / dy > maxValues_[1]) {
        maxValues_[1] = fabs(velocity[1]) / dy;
    }
    if (fabs(velocity[2]) / dz > maxValues_[2]) {
        maxValues_[2] = fabs(velocity[2]) / dz;
    }
}

void MaxUStencil::reset() {
    maxValues_[0] = 0;
    maxValues_[1] = 0;
    maxValues_[2] = 0;
}

const FLOAT* MaxUStencil::getMaxValues() const {
    return maxValues_;
}

} // namespace Stencils
} // namespace NSEOF
