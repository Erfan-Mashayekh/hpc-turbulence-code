#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void PressureBufferReadStencil::setPressureBuffers(
            std::vector<FLOAT>& pressureBufferLeft, std::vector<FLOAT>& pressureBufferRight,
            std::vector<FLOAT>& pressureBufferBottom, std::vector<FLOAT>& pressureBufferTop,
            std::vector<FLOAT>& pressureBufferFront, std::vector<FLOAT>& pressureBufferBack) {
        pressureLeftIterator_   = std::move(pressureBufferLeft).begin();
        pressureRightIterator_  = std::move(pressureBufferRight).begin();
        pressureBottomIterator_ = std::move(pressureBufferBottom).begin();
        pressureTopIterator_    = std::move(pressureBufferTop).begin();
        pressureFrontIterator_  = std::move(pressureBufferFront).begin();
        pressureBackIterator_   = std::move(pressureBufferBack).begin();
    }

    /**
     * Functions for 3D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureLeftIterator_++);
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureRightIterator_++);
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBottomIterator_++);
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureTopIterator_++);
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureFrontIterator_++);
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBackIterator_++);
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
