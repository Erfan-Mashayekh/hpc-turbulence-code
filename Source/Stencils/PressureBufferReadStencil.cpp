#include "PressureBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    PressureBufferReadStencil::PressureBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    /**
     * Functions for 3D
     */

    //TODO: update for 3D (rightnow it works for 2D)

    void PressureBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        //std::cout << "Recv --> " << "i " << i << "\n";
        //std::cout << "Recv --> j: " << j << " j-1 " << j - 1 << "\n";
        flowField.getPressure().getScalar(i, j, k) = pressureBufferLeftIterator_[j];      
    }

    void PressureBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        //std::cout << "Recv --> " << "i " << i << "\n";
        flowField.getPressure().getScalar(i, j, k) = pressureBufferRightIterator_[j];
    }

    void PressureBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferBottomIterator_[i-1];
    }

    void PressureBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferTopIterator_[i-1];
    }

    void PressureBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferFrontIterator_[k-1];
    }

    void PressureBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getPressure().getScalar(i, j, k) = pressureBufferBackIterator_[k-1];
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
        std::copy(pressureBufferLeft.begin(), pressureBufferLeft.end(), std::back_inserter(pressureBufferLeftIterator_));
    }

    void PressureBufferReadStencil::setPressureBufferRightIterator(std::vector<FLOAT>& pressureBufferRight) {
        std::copy(pressureBufferRight.begin(), pressureBufferRight.end(), std::back_inserter(pressureBufferRightIterator_));
    }

    void PressureBufferReadStencil::setPressureBufferBottomIterator(std::vector<FLOAT>& pressureBufferBottom) {
        std::copy(pressureBufferBottom.begin(), pressureBufferBottom.end(), std::back_inserter(pressureBufferBottomIterator_));
    }

    void PressureBufferReadStencil::setPressureBufferTopIterator(std::vector<FLOAT>& pressureBufferTop) {
        std::copy(pressureBufferTop.begin(), pressureBufferTop.end(), std::back_inserter(pressureBufferTopIterator_));
    }

    void PressureBufferReadStencil::setPressureBufferFrontIterator(std::vector<FLOAT>& pressureBufferFront) {
        std::copy(pressureBufferFront.begin(), pressureBufferFront.end(), std::back_inserter(pressureBufferFrontIterator_));
    }

    void PressureBufferReadStencil::setPressureBufferBackIterator(std::vector<FLOAT>& pressureBufferBack) {
        std::copy(pressureBufferBack.begin(), pressureBufferBack.end(), std::back_inserter(pressureBufferBackIterator_));
    }

} // namespace NSEOF::Stencils
