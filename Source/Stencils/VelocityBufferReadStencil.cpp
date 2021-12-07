#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    /**
     * Functions for 3D
     */

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i - 1, j, k)[0] = *(velocityBufferLeftIterator_++); // "u" 
        flowField.getVelocity().getVector(i, j, k)[1] = *(velocityBufferLeftIterator_++);     // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = *(velocityBufferLeftIterator_++);     // "w"
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = *(velocityBufferRightIterator_++); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = *(velocityBufferRightIterator_++); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = *(velocityBufferRightIterator_++); // "w"
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = *(velocityBufferBottomIterator_++);     // "u" 
        flowField.getVelocity().getVector(i, j - 1, k)[1] = *(velocityBufferBottomIterator_++); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = *(velocityBufferBottomIterator_++);     // "w"
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = *(velocityBufferTopIterator_++); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = *(velocityBufferTopIterator_++); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = *(velocityBufferTopIterator_++); // "w"
    }

    void VelocityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = *(velocityBufferFrontIterator_++);     // "u" 
        flowField.getVelocity().getVector(i, j, k)[1] = *(velocityBufferFrontIterator_++);     // "v"
        flowField.getVelocity().getVector(i, j, k - 1)[2] = *(velocityBufferFrontIterator_++); // "w"
    }

    void VelocityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = *(velocityBufferBackIterator_++); // "u"
        flowField.getVelocity().getVector(i, j, k)[1] = *(velocityBufferBackIterator_++); // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = *(velocityBufferBackIterator_++); // "w"
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
