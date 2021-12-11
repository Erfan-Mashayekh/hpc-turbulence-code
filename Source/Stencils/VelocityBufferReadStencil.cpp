#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BufferReadStencil(parameters) {}

    /**
     * Functions for 3D
     */

    // TODO: Generalize for 3D (it works for 2D)

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i - 1, j, k)[0] = getNextInBufferLeftAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1]     = getNextInBufferLeftAndIncrement(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2]     = getNextInBufferLeftAndIncrement(); // "w"
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferRightAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferRightAndIncrement(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferRightAndIncrement(); // "w"
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0]     = getNextInBufferBottomAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j - 1, k)[1] = getNextInBufferBottomAndIncrement(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2]     = getNextInBufferBottomAndIncrement(); // "w"
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferTopAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferTopAndIncrement(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferTopAndIncrement(); // "w"
    }

    void VelocityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0]     = getNextInBufferFrontAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1]     = getNextInBufferFrontAndIncrement(); // "v"
        flowField.getVelocity().getVector(i, j, k - 1)[2] = getNextInBufferFrontAndIncrement(); // "w"
    }

    void VelocityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferBackAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferBackAndIncrement(); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferBackAndIncrement(); // "w"
    }

    /**
     * Functions for 2D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j)[0] = getNextInBufferLeftAndIncrement(); // "u-"
        flowField.getVelocity().getVector(i, j)[1] = getNextInBufferLeftAndIncrement(); // "v"
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j)[0] = getNextInBufferRightAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j)[1] = getNextInBufferRightAndIncrement(); // "v"
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j - 1)[1] = getNextInBufferBottomAndIncrement(); // "v-"
        flowField.getVelocity().getVector(i, j)[0]     = getNextInBufferBottomAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j)[1]     = getNextInBufferBottomAndIncrement(); // "v"
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j)[0] = getNextInBufferTopAndIncrement(); // "u"
        flowField.getVelocity().getVector(i, j)[1] = getNextInBufferTopAndIncrement(); // "v"
    }

} // namespace NSEOF::Stencils
