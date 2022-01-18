#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(const Parameters& parameters)
        : BufferReadStencil(parameters) {}

    /**
     * Functions for 3D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferLeft();
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferRight();
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferBottom();
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferTop();
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferFront();
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = getNextInBufferBack();
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
