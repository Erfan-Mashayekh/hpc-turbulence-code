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

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferLeft().push_back(flowField.getVelocity().getVector(i + 1, j, k)[2]); // "w"
        }
    }

    void VelocityBufferFillStencil::applyRightWall(FlowField& flowField, int i, int j, int k) {
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 2, j, k)[0]); // "u"
        getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j, k)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferRight().push_back(flowField.getVelocity().getVector(i - 1, j, k)[2]); // "w"
        }    
    }
    
    void VelocityBufferFillStencil::applyBottomWall(FlowField& flowField, int i, int j, int k) {
        getBufferBottom().push_back(flowField.getVelocity().getVector(i, j + 1, k)[0]); // "u"
        getBufferBottom().push_back(flowField.getVelocity().getVector(i, j + 1, k)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferBottom().push_back(flowField.getVelocity().getVector(i, j + 1, k)[2]); // "w"
        }    
    }
    
    void VelocityBufferFillStencil::applyTopWall(FlowField& flowField, int i, int j, int k) {
        getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 1, k)[0]); // "u" 
        getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 2, k)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferTop().push_back(flowField.getVelocity().getVector(i, j - 1, k)[2]); // "w"
        }    
    }

    void VelocityBufferFillStencil::applyFrontWall(FlowField& flowField, int i, int j, int k) {
        getBufferFront().push_back(flowField.getVelocity().getVector(i, j, k + 1)[0]); // "u"
        getBufferFront().push_back(flowField.getVelocity().getVector(i, j, k + 1)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferFront().push_back(flowField.getVelocity().getVector(i, j, k + 1)[2]); // "w"
        }    
    }
    
    void VelocityBufferFillStencil::applyBackWall(FlowField& flowField, int i, int j, int k) {
        getBufferBack().push_back(flowField.getVelocity().getVector(i, j, k - 1)[0]); // "u"
        getBufferBack().push_back(flowField.getVelocity().getVector(i, j, k - 1)[1]); // "v"

        if (parameters_.geometry.dim == 3) { // 3D
            getBufferBack().push_back(flowField.getVelocity().getVector(i, j, k - 2)[2]); // "w"
        }    
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

} // namespace NSEOF::Stencils
