#include "VelocityBufferDiagonalReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferDiagonalReadStencil::VelocityBufferDiagonalReadStencil(const Parameters& parameters)
            : BufferReadStencil(parameters) {}

    /**
     * Functions for 3D
     */

    void VelocityBufferDiagonalReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferLeft(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferLeft(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferLeft(); // "w"
        }
    }

    void VelocityBufferDiagonalReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferRight(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferRight(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferRight(); // "w"
        }
    }

    void VelocityBufferDiagonalReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferBottom(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferBottom(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferBottom(); // "w"
        }
    }

    void VelocityBufferDiagonalReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = getNextInBufferTop(); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = getNextInBufferTop(); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            flowField.getVelocity().getVector(i, j, k)[2] = getNextInBufferTop(); // "w"
        }
    }

    void VelocityBufferDiagonalReadStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}

    void VelocityBufferDiagonalReadStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}

    /**
     * Functions for 2D
     */

    void VelocityBufferDiagonalReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void VelocityBufferDiagonalReadStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void VelocityBufferDiagonalReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void VelocityBufferDiagonalReadStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
    }

} // namespace NSEOF::Stencils
