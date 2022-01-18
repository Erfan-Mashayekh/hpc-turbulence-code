#include "PressureBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferFillStencil::PressureBufferFillStencil(const Parameters& parameters)
        : BufferFillStencil(parameters) {}

    /**
     * Functions for 3D
     */

    void PressureBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        getBufferLeft().push_back(flowField.getPressure().getScalar(i + 1, j, k));
    }

    void PressureBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        getBufferRight().push_back(flowField.getPressure().getScalar(i - 1, j, k));
    }

    void PressureBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        getBufferBottom().push_back(flowField.getPressure().getScalar(i, j + 1, k));
    }

    void PressureBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        getBufferTop().push_back(flowField.getPressure().getScalar(i, j - 1, k));
    }

    void PressureBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        getBufferFront().push_back(flowField.getPressure().getScalar(i, j, k + 1));
    }

    void PressureBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        getBufferBack().push_back(flowField.getPressure().getScalar(i, j, k - 1));
    }

    /**
     * Functions for 2D
     */

    void PressureBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void PressureBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void PressureBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void PressureBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
    }

} // namespace NSEOF::Stencils
