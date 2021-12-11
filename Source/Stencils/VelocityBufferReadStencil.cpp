#include "VelocityBufferReadStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferReadStencil::VelocityBufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    /**
     * Functions for 3D
     */

    // TODO: Generalize for 3D (it works for 2D)

    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i - 1, j, k)[0] = velocityBufferLeftIterator_[3 * j - 3]; // "u" 
        flowField.getVelocity().getVector(i, j, k)[1] = velocityBufferLeftIterator_[3 * j - 2];     // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = velocityBufferLeftIterator_[3 * j - 1];     // "w"
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        
        flowField.getVelocity().getVector(i, j, k)[0] = velocityBufferRightIterator_[3 * j - 3];     // "u" 
        flowField.getVelocity().getVector(i, j, k)[1] = velocityBufferRightIterator_[3 * j - 2];     // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = velocityBufferRightIterator_[3 * j - 1];     // "w"
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = velocityBufferBottomIterator_[3 * i - 3];     // "u" 
        flowField.getVelocity().getVector(i, j - 1, k)[1] = velocityBufferBottomIterator_[3 * i - 2]; // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = velocityBufferBottomIterator_[3 * i - 1];     // "w"
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = velocityBufferTopIterator_[3 * i - 3];     // "u" 
        flowField.getVelocity().getVector(i, j, k)[1] = velocityBufferTopIterator_[3 * i - 2];     // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = velocityBufferTopIterator_[3 * i - 1];     // "w"
    }

    void VelocityBufferReadStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = velocityBufferFrontIterator_[3 * k - 3];     // "u" 
        flowField.getVelocity().getVector(i, j, k)[1] = velocityBufferFrontIterator_[3 * k - 2];     // "v"
        flowField.getVelocity().getVector(i, j, k - 1)[2] = velocityBufferFrontIterator_[3 * k - 1]; // "w"
    }

    void VelocityBufferReadStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        flowField.getVelocity().getVector(i, j, k)[0] = velocityBufferBackIterator_[3 * k - 3];     // "u" 
        flowField.getVelocity().getVector(i, j, k)[1] = velocityBufferBackIterator_[3 * k - 2];     // "v"
        flowField.getVelocity().getVector(i, j, k)[2] = velocityBufferBackIterator_[3 * k - 1];     // "w"
    }

    /**
     * Functions for 2D
     */
    void VelocityBufferReadStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j)[0] = velocityBufferLeftIterator_[2 * j - 2];     // "u-"  
        flowField.getVelocity().getVector(i, j)[1] = velocityBufferLeftIterator_[2 * j - 1];     // "v"
        //std::cout << "Recv Left --> " << parameters_.parallel.rank << " " 
        //          << i << " " << j << " " 
        //          << flowField.getVelocity().getVector(i, j)[0]  
        //          <<  " length : " << velocityBufferLeftIterator_.size() << "\n";
    }

    void VelocityBufferReadStencil::applyRightWall(FlowField& flowField, int i, int j) {
        //std::cout << "Recv --> i: " << i << "\n";
        flowField.getVelocity().getVector(i, j)[0] = velocityBufferRightIterator_[2 * j - 2];     // "u" 
        flowField.getVelocity().getVector(i, j)[1] = velocityBufferRightIterator_[2 * j - 1];     // "v"
        //std::cout << "Recv Right <-- " << parameters_.parallel.rank << " " 
        //          << i << " " << j << " " 
        //          << flowField.getVelocity().getVector(i, j)[0]  
        //          << " length : " << velocityBufferRightIterator_.size() << "\n";
    }

    void VelocityBufferReadStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j - 1)[1] = velocityBufferBottomIterator_[3 * i - 3]; // "v-"
        flowField.getVelocity().getVector(i, j)[0] = velocityBufferBottomIterator_[3 * i - 2];     // "u" 
        flowField.getVelocity().getVector(i, j)[1] = velocityBufferBottomIterator_[3 * i - 1];     // "v"
    }

    void VelocityBufferReadStencil::applyTopWall(FlowField& flowField, int i, int j) {
        flowField.getVelocity().getVector(i, j)[0] = velocityBufferTopIterator_[2 * i - 2];     // "u" 
        flowField.getVelocity().getVector(i, j)[1] = velocityBufferTopIterator_[2 * i - 1];     // "v"
    }

    /**
     * Setters for velocity buffer iterators
     */

    void VelocityBufferReadStencil::setVelocityBufferLeftIterator(std::vector<FLOAT>& velocityBufferLeft) {
        velocityBufferLeftIterator_.assign(velocityBufferLeft.begin(), velocityBufferLeft.end());
    }

    void VelocityBufferReadStencil::setVelocityBufferRightIterator(std::vector<FLOAT>& velocityBufferRight) {
        velocityBufferRightIterator_.assign(velocityBufferRight.begin(), velocityBufferRight.end());
    }

    void VelocityBufferReadStencil::setVelocityBufferBottomIterator(std::vector<FLOAT>& velocityBufferBottom) {
        std::copy(velocityBufferBottom.begin(), velocityBufferBottom.end(), std::back_inserter(velocityBufferBottomIterator_));
    }

    void VelocityBufferReadStencil::setVelocityBufferTopIterator(std::vector<FLOAT>& velocityBufferTop) {
        std::copy(velocityBufferTop.begin(), velocityBufferTop.end(), std::back_inserter(velocityBufferTopIterator_));
    }

    void VelocityBufferReadStencil::setVelocityBufferFrontIterator(std::vector<FLOAT>& velocityBufferFront) {
        std::copy(velocityBufferFront.begin(), velocityBufferFront.end(), std::back_inserter(velocityBufferFrontIterator_));
    }

    void VelocityBufferReadStencil::setVelocityBufferBackIterator(std::vector<FLOAT>& velocityBufferBack) {
        std::copy(velocityBufferBack.begin(), velocityBufferBack.end(), std::back_inserter(velocityBufferBackIterator_));
    }

} // namespace NSEOF::Stencils
