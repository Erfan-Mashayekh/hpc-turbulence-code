#ifndef __STENCILS_PRESSURE_BUFFER_FILL_STENCIL_HPP__
#define __STENCILS_PRESSURE_BUFFER_FILL_STENCIL_HPP__

#include "BufferFillStencil.hpp"

namespace NSEOF::Stencils {

/**
 * A boundary stencil that fills the pressure buffer
 */
class PressureBufferFillStencil : public BufferFillStencil {
public:
    explicit PressureBufferFillStencil(const Parameters&);
    ~PressureBufferFillStencil() override = default;

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

#endif // __STENCILS_PRESSURE_BUFFER_FILL_STENCIL_HPP__
