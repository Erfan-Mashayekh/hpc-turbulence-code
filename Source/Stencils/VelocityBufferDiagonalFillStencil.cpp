#include "VelocityBufferDiagonalFillStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferDiagonalFillStencil::VelocityBufferDiagonalFillStencil(const Parameters& parameters)
            : BufferFillStencil(parameters) {}

    void VelocityBufferDiagonalFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j - 1, k)[0]); // "u"
        getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j - 1, k)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j, k)[2]); // "w"
        }
    }

    void VelocityBufferDiagonalFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j + 1, k)[0]); // "u"
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j + 1, k)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j, k)[2]); // "w"
        }
    }

    void VelocityBufferDiagonalFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        getBufferBottom().push_back(flowField.getVelocity().getVector(i + 1, j + 1, k)[0]); // "u"
        getBufferBottom().push_back(flowField.getVelocity().getVector(i + 1, j + 1, k)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferBottom().push_back(flowField.getVelocity().getVector(i + 1, j + 1, k)[2]); // "w"
        }
    }

    void VelocityBufferDiagonalFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        getBufferTop().push_back(flowField.getVelocity().getVector(i - 1, j - 1, k)[0]); // "u"
        getBufferTop().push_back(flowField.getVelocity().getVector(i - 1, j - 1, k)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferTop().push_back(flowField.getVelocity().getVector(i - 1, j - 1, k)[2]); // "w"
        }
    }

    void VelocityBufferDiagonalFillStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}

    void VelocityBufferDiagonalFillStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}

    /**
     * Functions for 2D
     */

    void VelocityBufferDiagonalFillStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void VelocityBufferDiagonalFillStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void VelocityBufferDiagonalFillStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void VelocityBufferDiagonalFillStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
    }

} // namespace NSEOF::Stencils
