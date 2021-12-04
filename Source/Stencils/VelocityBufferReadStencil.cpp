#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void VelocityBufferReadStencil::setVelocityBuffers(
            std::vector<FLOAT>& velocityBufferLeft, std::vector<FLOAT>& velocityBufferRight,
            std::vector<FLOAT>& velocityBufferBottom, std::vector<FLOAT>& velocityBufferTop,
            std::vector<FLOAT>& velocityBufferFront, std::vector<FLOAT>& velocityBufferBack) {
        velocityLeftIterator_   = std::move(velocityBufferLeft).begin();
        velocityRightIterator_  = std::move(velocityBufferRight).begin();
        velocityBottomIterator_ = std::move(velocityBufferBottom).begin();
        velocityTopIterator_    = std::move(velocityBufferTop).begin();
        velocityFrontIterator_  = std::move(velocityBufferFront).begin();
        velocityBackIterator_   = std::move(velocityBufferBack).begin();
    }

    /**
     * Functions for 3D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocity = flowField.getVelocity().getVector(i + 1, j, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocity[d] = *(velocityLeftIterator_++);
        }
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i - 2, j, k);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i - 1, j, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityLeft[d] = *(velocityRightIterator_++);
        }

        velocityRight[0] = *(velocityRightIterator_++); // only "u"
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        FLOAT *velocity = flowField.getVelocity().getVector(i, j + 1, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocity[d] = *(velocityBottomIterator_++);
        }
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i, j - 2, k);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i, j - 1, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityLeft[d] = *(velocityTopIterator_++);
        }

        velocityRight[0] = *(velocityTopIterator_++); // only "v"
    }

    void VelocityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i, j, k - 2);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i, j, k - 1);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityLeft[d] = *(velocityFrontIterator_++);
        }

        velocityRight[0] = *(velocityFrontIterator_++); // only "w"
    }

    void VelocityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        FLOAT *velocity = flowField.getVelocity().getVector(i, j, k + 1);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocity[d] = *(velocityBackIterator_++);
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
