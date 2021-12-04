#include "PressureBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferFillStencil::PressureBufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void PressureBufferFillStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void PressureBufferFillStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void PressureBufferFillStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void PressureBufferFillStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}

    void PressureBufferFillStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferFillStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferFillStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferFillStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferFillStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferFillStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
} // namespace NSEOF::Stencils
