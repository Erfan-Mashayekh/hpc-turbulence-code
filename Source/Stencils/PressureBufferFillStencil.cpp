#include "PressureBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferFillStencil::PressureBufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    PressureBufferFillStencil::~PressureBufferFillStencil() {
        pressuresLeft_.clear();
        pressuresRight_.clear();
        pressuresBottom_.clear();
        pressuresTop_.clear();
        pressuresFront_.clear();
        pressuresBack_.clear();
    }

    /**
     * Functions for 3D
     */

    static void applyWall(FlowField& flowField, int i, int j, int k, std::vector<FLOAT>& pressures) {
        pressures.push_back(flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresLeft_);
    }

    void PressureBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresRight_);
    }

    void PressureBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresBottom_);
    }

    void PressureBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresTop_);
    }

    void PressureBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresFront_);
    }

    void PressureBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresBack_);
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
