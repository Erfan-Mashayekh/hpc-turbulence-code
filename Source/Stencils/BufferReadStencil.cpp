#include "BufferReadStencil.hpp"

namespace NSEOF::Stencils {

    BufferReadStencil::BufferReadStencil(const Parameters& parameters)
            : BoundaryStencil<FlowField>(parameters) {}

    void BufferReadStencil::clearBuffers() {
        bufferLeftIterator_ = bufferLeft_.end();
        bufferRightIterator_ = bufferRight_.end();
        bufferBottomIterator_ = bufferBottom_.end();
        bufferTopIterator_ = bufferTop_.end();
        bufferFrontIterator_ = bufferFront_.end();
        bufferBackIterator_ = bufferBack_.end();

        bufferLeft_.clear();
        bufferRight_.clear();
        bufferBottom_.clear();
        bufferTop_.clear();
        bufferFront_.clear();
        bufferBack_.clear();
    }

    BufferReadStencil::~BufferReadStencil() {
        clearBuffers();
    }

    /**
     * Functions for 3D
     */

    void BufferReadStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferReadStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferReadStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferReadStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferReadStencil::applyFrontWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}
    void BufferReadStencil::applyBackWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) {}

    /**
     * Functions for 2D
     */

    void BufferReadStencil::applyLeftWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void BufferReadStencil::applyRightWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void BufferReadStencil::applyBottomWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}
    void BufferReadStencil::applyTopWall([[maybe_unused]] FlowField& flowField, [[maybe_unused]] int i, [[maybe_unused]] int j) {}

    /**
     * Gets the next value in the iterator and increments
     */

    FLOAT BufferReadStencil::getNextInBufferLeftAndIncrement  () { return *(bufferLeftIterator_++);   }
    FLOAT BufferReadStencil::getNextInBufferRightAndIncrement () { return *(bufferRightIterator_++);  }
    FLOAT BufferReadStencil::getNextInBufferBottomAndIncrement() { return *(bufferBottomIterator_++); }
    FLOAT BufferReadStencil::getNextInBufferTopAndIncrement   () { return *(bufferTopIterator_++);    }
    FLOAT BufferReadStencil::getNextInBufferFrontAndIncrement () { return *(bufferFrontIterator_++);  }
    FLOAT BufferReadStencil::getNextInBufferBackAndIncrement  () { return *(bufferBackIterator_++);   }

    /**
     * Setters for buffers and their iterators
     */

    void BufferReadStencil::setBufferLeftIterator(std::vector<FLOAT>& bufferLeft) {
        bufferLeft_.assign(bufferLeft.begin(), bufferLeft.end()); // Copy
        bufferLeftIterator_ = bufferLeft_.begin();
    }

    void BufferReadStencil::setBufferRightIterator(std::vector<FLOAT>& bufferRight) {
        bufferRight_.assign(bufferRight.begin(), bufferRight.end()); // Copy
        bufferRightIterator_ = bufferRight_.begin();
    }

    void BufferReadStencil::setBufferBottomIterator(std::vector<FLOAT>& bufferBottom) {
        bufferBottom_.assign(bufferBottom.begin(), bufferBottom.end()); // Copy
        bufferBottomIterator_ = bufferBottom_.begin();
    }

    void BufferReadStencil::setBufferTopIterator(std::vector<FLOAT>& bufferTop) {
        bufferTop_.assign(bufferTop.begin(), bufferTop.end()); // Copy
        bufferTopIterator_ = bufferTop_.begin();
    }

    void BufferReadStencil::setBufferFrontIterator(std::vector<FLOAT>& bufferFront) {
        bufferFront_.assign(bufferFront.begin(), bufferFront.end()); // Copy
        bufferFrontIterator_ = bufferFront_.begin();
    }

    void BufferReadStencil::setBufferBackIterator(std::vector<FLOAT>& bufferBack) {
        bufferBack_.assign(bufferBack.begin(), bufferBack.end()); // Copy
        bufferBackIterator_ = bufferBack_.begin();
    }

} // namespace NSEOF::Stencils
