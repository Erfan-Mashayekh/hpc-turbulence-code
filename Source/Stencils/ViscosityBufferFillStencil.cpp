#include "ViscosityBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    ViscosityBufferFillStencil::ViscosityBufferFillStencil(const Parameters& parameters)
        : BufferFillStencil(parameters) {}

    /**
     * Functions for 3D
     */

    void ViscosityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        getBufferLeft().push_back(flowField.getEddyViscosity().getScalar(i + 1, j, k));
    }

    void ViscosityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        getBufferRight().push_back(flowField.getEddyViscosity().getScalar(i - 1, j, k));
    }

    void ViscosityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        getBufferBottom().push_back(flowField.getEddyViscosity().getScalar(i, j + 1, k));
    }

    void ViscosityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        getBufferTop().push_back(flowField.getEddyViscosity().getScalar(i, j - 1, k));
    }

    void ViscosityBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        getBufferFront().push_back(flowField.getEddyViscosity().getScalar(i, j, k + 1));
    }

    void ViscosityBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        getBufferBack().push_back(flowField.getEddyViscosity().getScalar(i, j, k - 1));
    }

    /**
     * Functions for 2D
     */

    void ViscosityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void ViscosityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void ViscosityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void ViscosityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
    }

} // namespace NSEOF::Stencils
