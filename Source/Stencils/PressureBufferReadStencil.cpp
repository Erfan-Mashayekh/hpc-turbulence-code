#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(
            const Parameters& parameters,
            std::vector<FLOAT>& pressuresLeft, std::vector<FLOAT>& pressuresRight,
            std::vector<FLOAT>& pressuresBottom, std::vector<FLOAT>& pressuresTop,
            std::vector<FLOAT>& pressuresFront, std::vector<FLOAT>& pressuresBack)
            : BoundaryStencil<FlowField>(parameters)
            , pressuresLeft_(pressuresLeft), pressuresRight_(pressuresRight)
            , pressuresBottom_(pressuresBottom), pressuresTop_(pressuresTop)
            , pressuresFront_(pressuresFront), pressuresBack_(pressuresBack) {}

    /**
     * Functions for 3D
     */

    static void applyWall(FlowField& flowField, int i, int j, int k, std::vector<FLOAT>& pressures) {
        ScalarField& pressure = flowField.getPressure();
        pressure.getScalar(i, j, k) = pressures[pressure.index2array(i, j, k)];
    }

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresLeft_);
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresRight_);
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresBottom_);
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresTop_);
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresFront_);
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        applyWall(flowField, i, j, k, pressuresBack_);
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
