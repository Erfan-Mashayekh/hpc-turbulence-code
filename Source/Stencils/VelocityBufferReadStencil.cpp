#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BufferReadStencil(parameters) {}

    /**
     * Functions for 3D
     */

    // TODO: Generalize for 3D (it works for 2D)

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i - 1, j, k)[0] = getNextInBufferLeft(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1]     = getNextInBufferLeft(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2]     = getNextInBufferLeft(); // "w"
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferRight(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferRight(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferRight(); // "w"
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0]     = getNextInBufferBottom(); // "u"
        flowField.getVelocity().getVector(i, j - 1, k)[1] = getNextInBufferBottom(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2]     = getNextInBufferBottom(); // "w"
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferTop(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferTop(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferTop(); // "w"
    }

    void VelocityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0]     = getNextInBufferFront(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1]     = getNextInBufferFront(); // "v"
        flowField.getVelocity().getVector(i, j, k - 1)[2] = getNextInBufferFront(); // "w"
    }

    void VelocityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferBack(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferBack(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferBack(); // "w"
    }

    /**
     * Functions for 2D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j)[0] = getNextInBufferLeft(); // "u-"
        flowField.getVelocity().getVector(i, j)[1] = getNextInBufferLeft(); // "v"
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j)[0] = getNextInBufferRight(); // "u"
        flowField.getVelocity().getVector(i, j)[1] = getNextInBufferRight(); // "v"
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j - 1)[1] = getNextInBufferBottom(); // "v-"
        flowField.getVelocity().getVector(i, j)[0]     = getNextInBufferBottom(); // "u"
        flowField.getVelocity().getVector(i, j)[1]     = getNextInBufferBottom(); // "v"
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j)[0] = getNextInBufferTop(); // "u"
        flowField.getVelocity().getVector(i, j)[1] = getNextInBufferTop(); // "v"
    }

} // namespace NSEOF::Stencils
