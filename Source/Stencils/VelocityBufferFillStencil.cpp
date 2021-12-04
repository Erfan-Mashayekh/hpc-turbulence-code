#include "VelocityBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferFillStencil::VelocityBufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    /**
     * Functions for 3D
     */

    void VelocityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {}
    void VelocityBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {}

    /**
     * Functions for 2D
     */

    void VelocityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j) {}
    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j) {}
    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j) {}
    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j) {}

} // namespace NSEOF::Stencils
