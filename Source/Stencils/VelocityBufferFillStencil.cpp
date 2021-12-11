#include "VelocityBufferFillStencil.hpp"

namespace NSEOF::Stencils {

    VelocityBufferFillStencil::VelocityBufferFillStencil(const Parameters& parameters)
            : BufferFillStencil(parameters) {}

    /**
     * Functions for 3D
     */

    void VelocityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j, int k) {
        getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j, k)[0]); // "u"
        getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j, k)[1]); // "v"
        getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j, k)[2]); // "w"
    }

    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 2, j, k)[0]); // "u"
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j, k)[1]); // "v"
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j, k)[2]); // "w"
    }
    
    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        getBufferBottom().push_back(flowField.getVelocity().getVector(i, j + 1, k)[0]); // "u"
        getBufferBottom().push_back(flowField.getVelocity().getVector(i, j + 1, k)[1]); // "v"
        getBufferBottom().push_back(flowField.getVelocity().getVector(i, j + 1, k)[2]); // "w"
    }
    
    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 1, k)[0]); // "u" 
        getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 2, k)[1]); // "v"
        getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 1, k)[2]); // "w"
    }

    void VelocityBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        getBufferFront().push_back(flowField.getVelocity().getVector(i, j, k + 1)[0]); // "u"
        getBufferFront().push_back(flowField.getVelocity().getVector(i, j, k + 1)[1]); // "v"
        getBufferFront().push_back(flowField.getVelocity().getVector(i, j, k + 1)[2]); // "w"
    }
    
    void VelocityBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        getBufferBack().push_back(flowField.getVelocity().getVector(i, j, k - 1)[0]); // "u"
        getBufferBack().push_back(flowField.getVelocity().getVector(i, j, k - 1)[1]); // "v"
        getBufferBack().push_back(flowField.getVelocity().getVector(i, j, k - 2)[2]); // "w"
    }

    /**
     * Functions for 2D
     */
    void VelocityBufferFillStencil::applyLeftWall(FlowField& flowField, int i, int j) {
        getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j)[0]); // "u"
        getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j)[1]); // "v"
    }

    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j) {
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j)[0]); // "u-"
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j)[1]); // "v"
    }
    
    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j) {
        getBufferBottom().push_back(flowField.getVelocity().getVector(i, j + 1)[0]); // "u"
        getBufferBottom().push_back(flowField.getVelocity().getVector(i, j + 1)[1]); // "v"
    }
    
    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j) {
        getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 2)[1]); // "v-"
        getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 1)[0]); // "u" 
        getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 1)[1]); // "v"    
    }

} // namespace NSEOF::Stencils
