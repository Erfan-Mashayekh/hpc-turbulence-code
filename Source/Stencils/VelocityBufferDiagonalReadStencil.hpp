#ifndef __STENCILS_VELOCITY_BUFFER_DIAGONAL_READ_STENCIL_HPP__
#define __STENCILS_VELOCITY_BUFFER_DIAGONAL_READ_STENCIL_HPP__

#include "BufferReadStencil.hpp"

namespace NSEOF::Stencils {

/**
 * A boundary stencil that reads the velocity buffer (diagonally)
 */
class VelocityBufferDiagonalReadStencil : public BufferReadStencil {
public:
    explicit VelocityBufferDiagonalReadStencil(const Parameters&);
    ~VelocityBufferDiagonalReadStencil() override = default;

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
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VELOCITY_BUFFER_DIAGONAL_READ_STENCIL_HPP__
