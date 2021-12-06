#include "VelocityBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferFillStencil::VelocityBufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    VelocityBufferFillStencil::~VelocityBufferFillStencil() {
        velocityBufferLeft_.clear();
        velocityBufferRight_.clear();
        velocityBufferBottom_.clear();
        velocityBufferTop_.clear();
        velocityBufferFront_.clear();
        velocityBufferBack_.clear();
    }

    /**
     * Functions for 3D
     */

    void VelocityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocity = flowField.getVelocity().getVector(i + 1, j, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityBufferLeft_.push_back(velocity[d]);
        }
    }

    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i - 2, j, k);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i - 1, j, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityBufferRight_.push_back(velocityLeft[d]);
        }

        // velocityBufferRight_.push_back(velocityRight[0]); // only "u"
    }
    
    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocity = flowField.getVelocity().getVector(i, j + 1, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityBufferBottom_.push_back(velocity[d]);
        }
    }
    
    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i, j - 2, k);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i, j - 1, k);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityBufferTop_.push_back(velocityLeft[d]);
        }

        // velocityBufferTop_.push_back(velocityRight[1]); // only "v"
    }

    void VelocityBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocity = flowField.getVelocity().getVector(i, j, k + 1);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityBufferBack_.push_back(velocity[d]);
        }
    }
    
    void VelocityBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        FLOAT* velocityLeft = flowField.getVelocity().getVector(i, j, k - 2);
        FLOAT* velocityRight = flowField.getVelocity().getVector(i, j, k - 1);

        for (int d = 0; d < parameters_.geometry.dim; d++) { // "u", "v" and "w"
            velocityBufferFront_.push_back(velocityLeft[d]);
        }

        // velocityBufferFront_.push_back(velocityRight[2]); // only "w"
    }

    /**
     * Functions for 2D
     */

    void VelocityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
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
