#include "VelocityBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferFillStencil::VelocityBufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void VelocityBufferFillStencil::clearVelocityBuffers() {
        velocityBufferLeft_.clear();
        velocityBufferRight_.clear();
        velocityBufferBottom_.clear();
        velocityBufferTop_.clear();
        velocityBufferFront_.clear();
        velocityBufferBack_.clear();
    }

    VelocityBufferFillStencil::~VelocityBufferFillStencil() {
        clearVelocityBuffers();
    }

    /**
     * Functions for 3D
     */

    void VelocityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        velocityBufferLeft_.push_back(flowField.getVelocity().getVector(i + 1, j, k)[0]); // "u"
        velocityBufferLeft_.push_back(flowField.getVelocity().getVector(i + 1, j, k)[1]); // "v"
        velocityBufferLeft_.push_back(flowField.getVelocity().getVector(i + 1, j, k)[2]); // "w"
    }

    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        velocityBufferRight_.push_back(flowField.getVelocity().getVector(i - 2, j, k)[0]); // "u"
        velocityBufferRight_.push_back(flowField.getVelocity().getVector(i - 1, j, k)[1]); // "v"
        velocityBufferRight_.push_back(flowField.getVelocity().getVector(i - 1, j, k)[2]); // "w"
    }
    
    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        velocityBufferBottom_.push_back(flowField.getVelocity().getVector(i, j + 1, k)[0]); // "u"
        velocityBufferBottom_.push_back(flowField.getVelocity().getVector(i, j + 1, k)[1]); // "v"
        velocityBufferBottom_.push_back(flowField.getVelocity().getVector(i, j + 1, k)[2]); // "w"
    }
    
    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        velocityBufferTop_.push_back(flowField.getVelocity().getVector(i, j - 1, k)[0]); // "u" 
        velocityBufferTop_.push_back(flowField.getVelocity().getVector(i, j - 2, k)[1]); // "v"
        velocityBufferTop_.push_back(flowField.getVelocity().getVector(i, j - 1, k)[2]); // "w"
    }

    void VelocityBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        velocityBufferFront_.push_back(flowField.getVelocity().getVector(i, j, k + 1)[0]); // "u"
        velocityBufferFront_.push_back(flowField.getVelocity().getVector(i, j, k + 1)[1]); // "v"
        velocityBufferFront_.push_back(flowField.getVelocity().getVector(i, j, k + 1)[2]); // "w"
    }
    
    void VelocityBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        velocityBufferBack_.push_back(flowField.getVelocity().getVector(i, j, k - 1)[0]); // "u"
        velocityBufferBack_.push_back(flowField.getVelocity().getVector(i, j, k - 1)[1]); // "v"
        velocityBufferBack_.push_back(flowField.getVelocity().getVector(i, j, k - 2)[2]); // "w"
    }

    /**
     * Functions for 2D
     */
    void VelocityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        velocityBufferLeft_.push_back(flowField.getVelocity().getVector(i + 1, j)[0]); // "u"
        velocityBufferLeft_.push_back(flowField.getVelocity().getVector(i + 1, j)[1]); // "v"
    }

    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j) {
        velocityBufferRight_.push_back(flowField.getVelocity().getVector(i - 1, j)[0]); // "u-"
        velocityBufferRight_.push_back(flowField.getVelocity().getVector(i - 1, j)[1]); // "v"
    }
    
    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        velocityBufferBottom_.push_back(flowField.getVelocity().getVector(i, j + 1)[0]); // "u"
        velocityBufferBottom_.push_back(flowField.getVelocity().getVector(i, j + 1)[1]); // "v"
    }
    
    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j) {
        velocityBufferTop_.push_back(flowField.getVelocity().getVector(i, j - 2)[1]); // "v-"
        velocityBufferTop_.push_back(flowField.getVelocity().getVector(i, j - 1)[0]); // "u" 
        velocityBufferTop_.push_back(flowField.getVelocity().getVector(i, j - 1)[1]); // "v"    
    }

    /**
     * Getters for velocity buffers
     */

    const std::vector<FLOAT>& VelocityBufferFillStencil::getVelocityBufferLeft() const {
        return velocityBufferLeft_;
    }

    const std::vector<FLOAT>& VelocityBufferFillStencil::getVelocityBufferRight() const {
        return velocityBufferRight_;
    }

    const std::vector<FLOAT>& VelocityBufferFillStencil::getVelocityBufferBottom() const {
        return velocityBufferBottom_;
    }

    const std::vector<FLOAT>& VelocityBufferFillStencil::getVelocityBufferTop() const {
        return velocityBufferTop_;
    }

    const std::vector<FLOAT>& VelocityBufferFillStencil::getVelocityBufferFront() const {
        return velocityBufferFront_;
    }

    const std::vector<FLOAT>& VelocityBufferFillStencil::getVelocityBufferBack() const {
        return velocityBufferBack_;
    }

} // namespace NSEOF::Stencils
