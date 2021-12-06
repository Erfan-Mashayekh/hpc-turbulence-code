#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    /**
     * Functions for 3D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocity = flowField.getVelocity().getVector(i + 1, j, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocity[d] = *(velocityBufferLeftIterator_++);
        }
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i - 2, j, k);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i - 1, j, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityLeft[d] = *(velocityBufferRightIterator_++);
        }

        // velocityRight[0] = *(velocityBufferRightIterator_++); // only "u"
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        FLOAT *velocity = flowField.getVelocity().getVector(i, j + 1, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocity[d] = *(velocityBufferBottomIterator_++);
        }
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i, j - 2, k);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i, j - 1, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityLeft[d] = *(velocityBufferTopIterator_++);
        }

        // velocityRight[0] = *(velocityBufferTopIterator_++); // only "v"
    }

    void VelocityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        FLOAT *velocity = flowField.getVelocity().getVector(i, j, k + 1);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocity[d] = *(velocityBufferBackIterator_++);
        }
    }

    void VelocityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i, j, k - 2);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i, j, k - 1);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityLeft[d] = *(velocityBufferFrontIterator_++);
        }

        // velocityRight[0] = *(velocityBufferFrontIterator_++); // only "w"
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

    /**
     * Setters for velocity buffer iterators
     */

    void VelocityBufferReadStencil::setVelocityBufferLeftIterator(std::vector<FLOAT>& velocityBufferLeft) {
        velocityBufferLeftIterator_ = std::move(velocityBufferLeft).begin();
    }

    void VelocityBufferReadStencil::setVelocityBufferRightIterator(std::vector<FLOAT>& velocityBufferRight) {
        velocityBufferRightIterator_ = std::move(velocityBufferRight).begin();
    }

    void VelocityBufferReadStencil::setVelocityBufferBottomIterator(std::vector<FLOAT>& velocityBufferBottom) {
        velocityBufferBottomIterator_ = std::move(velocityBufferBottom).begin();
    }

    void VelocityBufferReadStencil::setVelocityBufferTopIterator(std::vector<FLOAT>& velocityBufferTop) {
        velocityBufferTopIterator_ = std::move(velocityBufferTop).begin();
    }

    void VelocityBufferReadStencil::setVelocityBufferFrontIterator(std::vector<FLOAT>& velocityBufferFront) {
        velocityBufferFrontIterator_ = std::move(velocityBufferFront).begin();
    }

    void VelocityBufferReadStencil::setVelocityBufferBackIterator(std::vector<FLOAT>& velocityBufferBack) {
        velocityBufferBackIterator_ = std::move(velocityBufferBack).begin();
    }

} // namespace NSEOF::Stencils
