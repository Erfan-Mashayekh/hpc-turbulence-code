#include "PressureBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferFillStencil::PressureBufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void PressureBufferFillStencil::clearPressureBuffers() {
        pressureBufferLeft_.clear();
        pressureBufferRight_.clear();
        pressureBufferBottom_.clear();
        pressureBufferTop_.clear();
        pressureBufferFront_.clear();
        pressureBufferBack_.clear();
    }

    PressureBufferFillStencil::~PressureBufferFillStencil() {
        clearPressureBuffers();
    }

    /**
     * Functions for 3D
     */

    void PressureBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        // pressureBufferLeft_.push_back(flowField.getPressure().getScalar(i + 1, j, k));
        pressureBufferLeft_.push_back(100 + j);
    }

    void PressureBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        // pressureBufferRight_.push_back(flowField.getPressure().getScalar(i - 1, j, k));
        pressureBufferRight_.push_back(200 + j);
    }

    void PressureBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferBottom_.push_back(flowField.getPressure().getScalar(i, j + 1, k));
    }

    void PressureBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferTop_.push_back(flowField.getPressure().getScalar(i, j - 1, k));
    }

    void PressureBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferFront_.push_back(flowField.getPressure().getScalar(i, j, k + 1));
    }

    void PressureBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        pressureBufferBack_.push_back(flowField.getPressure().getScalar(i, j, k - 1));
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

    /**
     * Getters for pressure buffers
     */

    const std::vector<FLOAT>& PressureBufferFillStencil::getPressureBufferLeft() const {
        return pressureBufferLeft_;
    }

    const std::vector<FLOAT>& PressureBufferFillStencil::getPressureBufferRight() const {
        return pressureBufferRight_;
    }

    const std::vector<FLOAT>& PressureBufferFillStencil::getPressureBufferBottom() const {
        return pressureBufferBottom_;
    }

    const std::vector<FLOAT>& PressureBufferFillStencil::getPressureBufferTop() const {
        return pressureBufferTop_;
    }

    const std::vector<FLOAT>& PressureBufferFillStencil::getPressureBufferFront() const {
        return pressureBufferFront_;
    }

    const std::vector<FLOAT>& PressureBufferFillStencil::getPressureBufferBack() const {
        return pressureBufferBack_;
    }

} // namespace NSEOF::Stencils
