#ifndef __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__
#define __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__

#include "BufferReadStencil.hpp"

namespace NSEOF::Stencils {

/**
 * A boundary stencil that reads the pressure buffer
 */
class PressureBufferReadStencil : public BufferReadStencil {
public:
    explicit PressureBufferReadStencil(const Parameters&);
    ~PressureBufferReadStencil() override = default;

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

#endif // __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__
