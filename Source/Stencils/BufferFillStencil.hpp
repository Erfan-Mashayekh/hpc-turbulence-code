#ifndef __STENCILS_BUFFER_FILL_STENCIL_HPP__
#define __STENCILS_BUFFER_FILL_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <vector>

namespace NSEOF::Stencils {

/**
 * A boundary stencil that fills the buffer
 */
class BufferFillStencil : public BoundaryStencil<FlowField> {

private:
    std::vector<FLOAT> bufferLeft_;
    std::vector<FLOAT> bufferRight_;
    std::vector<FLOAT> bufferBottom_;
    std::vector<FLOAT> bufferTop_;
    std::vector<FLOAT> bufferFront_;
    std::vector<FLOAT> bufferBack_;

public:
    explicit BufferFillStencil(const Parameters&);

    void clearBuffers();
    ~BufferFillStencil() override;

    /**
     * Functions for 3D
     */
    void applyLeftWall  (FlowField&, int, int, int) override;
    void applyRightWall (FlowField&, int, int, int) override;
    void applyBottomWall(FlowField&, int, int, int) override;
    void applyTopWall   (FlowField&, int, int, int) override;
    void applyFrontWall (FlowField&, int, int, int) override;
    void applyBackWall  (FlowField&, int, int, int) override;

    /**
     * Functions for 2D
     */
    void applyLeftWall  (FlowField&, int, int) override;
    void applyRightWall (FlowField&, int, int) override;
    void applyBottomWall(FlowField&, int, int) override;
    void applyTopWall   (FlowField&, int, int) override;

    /**
     * Getters for velocity buffers
     */
    std::vector<FLOAT>& getBufferLeft  ();
    std::vector<FLOAT>& getBufferRight ();
    std::vector<FLOAT>& getBufferBottom();
    std::vector<FLOAT>& getBufferTop   ();
    std::vector<FLOAT>& getBufferFront ();
    std::vector<FLOAT>& getBufferBack  ();
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_BUFFER_FILL_STENCIL_HPP__
