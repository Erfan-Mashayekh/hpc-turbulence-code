#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void PressureBufferReadStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void PressureBufferReadStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void PressureBufferReadStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void PressureBufferReadStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}

    void PressureBufferReadStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferReadStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferReadStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferReadStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferReadStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void PressureBufferReadStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
} // namespace NSEOF::Stencils
