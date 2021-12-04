#include "VelocityBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferFillStencil::VelocityBufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void VelocityBufferFillStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void VelocityBufferFillStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void VelocityBufferFillStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void VelocityBufferFillStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}

    void VelocityBufferFillStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferFillStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferFillStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferFillStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferFillStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void VelocityBufferFillStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
} // namespace NSEOF::Stencils
