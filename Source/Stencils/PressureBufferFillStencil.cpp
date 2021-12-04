#include "PressureBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferFillStencil::PressureBufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    PressureBufferFillStencil::~PressureBufferFillStencil() {
        pressureBufferLeft_.clear();
        pressureBufferRight_.clear();
        pressureBufferBottom_.clear();
        pressureBufferTop_.clear();
        pressureBufferFront_.clear();
        pressureBufferBack_.clear();
    }

    void PressureBufferFillStencil::getPressureBuffers(
            std::vector<FLOAT>& pressureBufferLeft, std::vector<FLOAT>& pressureBufferRight,
            std::vector<FLOAT>& pressureBufferBottom, std::vector<FLOAT>& pressureBufferTop,
            std::vector<FLOAT>& pressureBufferFront, std::vector<FLOAT>& pressureBufferBack) {
        pressureBufferLeft_   = pressureBufferLeft;
        pressureBufferRight_  = pressureBufferRight;
        pressureBufferBottom_ = pressureBufferBottom;
        pressureBufferTop_    = pressureBufferTop;
        pressureBufferFront_  = pressureBufferFront;
        pressureBufferBack_   = pressureBufferBack;
    }

    /**
     * Functions for 3D
     */

    static void applyWall(FlowField& flowField, int i, int j, int k, std::vector<FLOAT>& pressureBuffer) {
        pressureBuffer.push_back(flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferLeft_);
    }

    void PressureBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferRight_);
    }

    void PressureBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferBottom_);
    }

    void PressureBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferTop_);
    }

    void PressureBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferFront_);
    }

    void PressureBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressureBufferBack_);
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
