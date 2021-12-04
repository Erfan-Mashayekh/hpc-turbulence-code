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

    static void applyWall(FlowField& flowField, int i, int j, int k, std::vector<FLOAT>& pressureBuffer) {
        ScalarField& pressure = flowField.getPressure();
        pressure.getScalar(i, j, k) = pressureBuffer[pressure.index2array(i, j, k)];
    }

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferLeft_);
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferRight_);
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferBottom_);
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferTop_);
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferFront_);
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferBack_);
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
