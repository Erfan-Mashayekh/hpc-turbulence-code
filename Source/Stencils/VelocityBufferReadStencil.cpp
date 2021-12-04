#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    /**
     * Functions for 3D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {}

    /**
     * Functions for 2D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {}
    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {}
    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {}
    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {}

} // namespace NSEOF::Stencils
