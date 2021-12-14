#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
        : BufferReadStencil(parameters) {}

    /**
     * Functions for 3D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i - 1, j, k)[0] = getNextInBufferLeft(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferLeft(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferLeft(); // "w"
        }
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferRight(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferRight(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferRight(); // "w"
        }
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferBottom(); // "u"
        flowField.getVelocity().getVector(i, j - 1, k)[1] = getNextInBufferBottom(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferBottom(); // "w"
        }
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferTop(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferTop(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferTop(); // "w"
        }
    }

    void VelocityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferFront(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferFront(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k - 1)[2] = getNextInBufferFront(); // "w"
        }
    }

    void VelocityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferBack(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferBack(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferBack(); // "w"
        }
    }

    /**
     * Functions for 2D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
    }

} // namespace NSEOF::Stencils
