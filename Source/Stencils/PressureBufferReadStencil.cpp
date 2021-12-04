#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    /**
     * Functions for 3D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferLeftIterator_++);
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferRightIterator_++);
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferBottomIterator_++);
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferTopIterator_++);
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferFrontIterator_++);
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferBackIterator_++);
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

    /**
     * Setters for the pressure buffer iterators
     */

    void PressureBufferReadStencil::setPressureBufferLeftIterator(std::vector<FLOAT>& pressureBufferLeft) {
        pressureBufferLeftIterator_ = std::move(pressureBufferLeft).begin();
    }

    void PressureBufferReadStencil::setPressureBufferRightIterator(std::vector<FLOAT>& pressureBufferRight) {
        pressureBufferRightIterator_ = std::move(pressureBufferRight).begin();
    }

    void PressureBufferReadStencil::setPressureBufferBottomIterator(std::vector<FLOAT>& pressureBufferBottom) {
        pressureBufferBottomIterator_ = std::move(pressureBufferBottom).begin();
    }

    void PressureBufferReadStencil::setPressureBufferTopIterator(std::vector<FLOAT>& pressureBufferTop) {
        pressureBufferTopIterator_ = std::move(pressureBufferTop).begin();
    }

    void PressureBufferReadStencil::setPressureBufferFrontIterator(std::vector<FLOAT>& pressureBufferFront) {
        pressureBufferFrontIterator_ = std::move(pressureBufferFront).begin();
    }

    void PressureBufferReadStencil::setPressureBufferBackIterator(std::vector<FLOAT>& pressureBufferBack) {
        pressureBufferBackIterator_ = std::move(pressureBufferBack).begin();
    }

} // namespace NSEOF::Stencils
