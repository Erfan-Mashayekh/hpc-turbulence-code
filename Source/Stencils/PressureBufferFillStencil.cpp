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

    void PressureBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferLeft_.push_back(flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferRight_.push_back(flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferBottom_.push_back(flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferTop_.push_back(flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferFront_.push_back(flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferBack_.push_back(flowField.getPressure().getScalar(i, j, k));
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
