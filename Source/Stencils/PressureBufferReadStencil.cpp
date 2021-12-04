#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    PressureBufferReadStencil::~PressureBufferReadStencil() {
        pressureBufferLeft_.clear();
        pressureBufferRight_.clear();
        pressureBufferBottom_.clear();
        pressureBufferTop_.clear();
        pressureBufferFront_.clear();
        pressureBufferBack_.clear();
    }

    void PressureBufferReadStencil::setPressureBuffers(
            std::vector<FLOAT>& pressureBufferLeft, std::vector<FLOAT>& pressureBufferRight,
            std::vector<FLOAT>& pressureBufferBottom, std::vector<FLOAT>& pressureBufferTop,
            std::vector<FLOAT>& pressureBufferFront, std::vector<FLOAT>& pressureBufferBack) {
        pressureBufferLeft_   = std::move(pressureBufferLeft);
        pressureBufferRight_  = std::move(pressureBufferRight);
        pressureBufferBottom_ = std::move(pressureBufferBottom);
        pressureBufferTop_    = std::move(pressureBufferTop);
        pressureBufferFront_  = std::move(pressureBufferFront);
        pressureBufferBack_   = std::move(pressureBufferBack);
    }

    /**
     * Functions for 3D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferLeft_[j];
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferRight_[j];
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferBottom_[i];
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferTop_[i];
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferFront_[k];
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferBack_[k];
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
