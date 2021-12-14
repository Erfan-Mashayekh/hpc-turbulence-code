#include "ViscosityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    ViscosityBufferReadStencil::ViscosityBufferReadStencil(const Parameters& parameters)
        : BufferReadStencil(parameters) {}

    /**
     * Functions for 3D
     */

    void ViscosityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getEddyViscosity().getScalar(i, j, k) = getNextInBufferLeft();
    }

    void ViscosityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getEddyViscosity().getScalar(i, j, k) = getNextInBufferRight();
    }

    void ViscosityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getEddyViscosity().getScalar(i, j, k) = getNextInBufferBottom();
    }

    void ViscosityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getEddyViscosity().getScalar(i, j, k) = getNextInBufferTop();
    }

    void ViscosityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getEddyViscosity().getScalar(i, j, k) = getNextInBufferFront();
    }

    void ViscosityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getEddyViscosity().getScalar(i, j, k) = getNextInBufferBack();
    }

    /**
     * Functions for 2D
     */

    void ViscosityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void ViscosityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void ViscosityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void ViscosityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
    }

} // namespace NSEOF::Stencils
