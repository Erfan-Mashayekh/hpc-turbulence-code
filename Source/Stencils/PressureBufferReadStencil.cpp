#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    PressureBufferReadStencil::~PressureBufferReadStencil() {
        pressureBufferLeftIterator_ = pressureBufferLeft_.end();
        pressureBufferRightIterator_ = pressureBufferRight_.end();
        pressureBufferBottomIterator_ = pressureBufferBottom_.end();
        pressureBufferTopIterator_ = pressureBufferTop_.end();
        pressureBufferFrontIterator_ = pressureBufferFront_.end();
        pressureBufferBackIterator_ = pressureBufferBack_.end();

        pressureBufferLeft_.clear();
        pressureBufferRight_.clear();
        pressureBufferBottom_.clear();
        pressureBufferTop_.clear();
        pressureBufferFront_.clear();
        pressureBufferBack_.clear();
    }

    /**
     * Functions for 3D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferLeftIterator_++);

        printf("Rank %d reading its left at (%d, %d, %d): %f\n",
               parameters_.parallel.rank, i, j, k, flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferRightIterator_++);

        printf("Rank %d reading its right at (%d, %d, %d): %f\n",
               parameters_.parallel.rank, i, j, k, flowField.getPressure().getScalar(i, j, k));
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferBottomIterator_++);
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferTopIterator_++);
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferFrontIterator_++);
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = *(pressureBufferBackIterator_++);
    }

    /**
     * Functions for 2D
     */

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        applyLeftWall(flowField, i, j, 0);
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {
        applyRightWall(flowField, i, j, 0);
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        applyBottomWall(flowField, i, j, 0);
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {
        applyTopWall(flowField, i, j, 0);
    }

    /**
     * Setters for pressure buffer iterators
     */

    void PressureBufferReadStencil::setPressureBufferLeftIterator(std::vector<FLOAT>& pressureBufferLeft) {
        pressureBufferLeft_.assign(pressureBufferLeft.begin(), pressureBufferLeft.end()); // Copy
        pressureBufferLeftIterator_ = pressureBufferLeft_.begin();
    }

    void PressureBufferReadStencil::setPressureBufferRightIterator(std::vector<FLOAT>& pressureBufferRight) {
        pressureBufferRight_.assign(pressureBufferRight.begin(), pressureBufferRight.end()); // Copy
        pressureBufferRightIterator_ = pressureBufferRight_.begin();
    }

    void PressureBufferReadStencil::setPressureBufferBottomIterator(std::vector<FLOAT>& pressureBufferBottom) {
        pressureBufferBottom_.assign(pressureBufferBottom.begin(), pressureBufferBottom.end()); // Copy
        pressureBufferBottomIterator_ = pressureBufferBottom_.begin();
    }

    void PressureBufferReadStencil::setPressureBufferTopIterator(std::vector<FLOAT>& pressureBufferTop) {
        pressureBufferTop_.assign(pressureBufferTop.begin(), pressureBufferTop.end()); // Copy
        pressureBufferTopIterator_ = pressureBufferTop_.begin();
    }

    void PressureBufferReadStencil::setPressureBufferFrontIterator(std::vector<FLOAT>& pressureBufferFront) {
        pressureBufferFront_.assign(pressureBufferFront.begin(), pressureBufferFront.end()); // Copy
        pressureBufferFrontIterator_ = pressureBufferFront_.begin();
    }

    void PressureBufferReadStencil::setPressureBufferBackIterator(std::vector<FLOAT>& pressureBufferBack) {
        pressureBufferBack_.assign(pressureBufferBack.begin(), pressureBufferBack.end()); // Copy
        pressureBufferBackIterator_ = pressureBufferBack_.begin();
    }

} // namespace NSEOF::Stencils
