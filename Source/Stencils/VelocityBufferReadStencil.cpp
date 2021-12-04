#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    /**
     * Functions for 3D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {}
    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {}
    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {}
    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {}
    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {}
    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {}

    /**
     * Functions for 2D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {}
    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {}
    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {}
    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {}

} // namespace NSEOF::Stencils
