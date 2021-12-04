#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void VelocityBufferReadStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void VelocityBufferReadStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void VelocityBufferReadStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void VelocityBufferReadStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}

    void VelocityBufferReadStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferReadStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferReadStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferReadStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferReadStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferReadStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
} // namespace NSEOF::Stencils
