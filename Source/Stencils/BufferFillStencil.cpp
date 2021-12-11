#include "BufferFillStencil.hpp"

namespace NSEOF::Stencils {

    BufferFillStencil::BufferFillStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void BufferFillStencil::clearBuffers() {
        bufferLeft_.clear();
        bufferRight_.clear();
        bufferBottom_.clear();
        bufferTop_.clear();
        bufferFront_.clear();
        bufferBack_.clear();
    }

    BufferFillStencil::~BufferFillStencil() {
        clearBuffers();
    }

    /**
     * Functions for 3D
     */

    void BufferFillStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferFillStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferFillStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferFillStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferFillStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferFillStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}

    /**
     * Functions for 2D
     */

    void BufferFillStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void BufferFillStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void BufferFillStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void BufferFillStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}

    /**
     * Getters for buffers
     */

    std::vector<FLOAT>& BufferFillStencil::getBufferLeft  () { return bufferLeft_;   }
    std::vector<FLOAT>& BufferFillStencil::getBufferRight () { return bufferRight_;  }
    std::vector<FLOAT>& BufferFillStencil::getBufferBottom() { return bufferBottom_; }
    std::vector<FLOAT>& BufferFillStencil::getBufferTop   () { return bufferTop_;    }
    std::vector<FLOAT>& BufferFillStencil::getBufferFront () { return bufferFront_;  }
    std::vector<FLOAT>& BufferFillStencil::getBufferBack  () { return bufferBack_;   }

} // namespace NSEOF::Stencils
