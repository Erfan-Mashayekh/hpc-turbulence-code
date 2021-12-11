#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(const Parameters& parameters)
            : BufferReadStencil(parameters) {}

    /**
     * Functions for 3D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferLeftAndIncrement();
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferRightAndIncrement();
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferBottomAndIncrement();
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferTopAndIncrement();
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferFrontAndIncrement();
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferBackAndIncrement();
    }

    /**
     * Functions for 2D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
    }

} // namespace NSEOF::Stencils
