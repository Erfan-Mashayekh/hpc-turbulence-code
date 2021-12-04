#ifndef __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__
#define __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__

#include "Stencil.hpp"
#include "Parameters.hpp"
#include "FlowField.hpp"

namespace NSEOF::Stencils {

/**
 * TODO: Write a definition here!
 */
class VelocityBufferReadStencil : public BoundaryStencil<FlowField> {

public:
    explicit VelocityBufferReadStencil(const Parameters&);
    ~VelocityBufferReadStencil() override = default;

    void applyLeftWall   (FlowField&, int, int) override;
    void applyRightWall  (FlowField&, int, int) override;
    void applyBottomWall (FlowField&, int, int) override;
    void applyTopWall    (FlowField&, int, int) override;

    void applyLeftWall   (FlowField&, int, int, int) override;
    void applyRightWall  (FlowField&, int, int, int) override;
    void applyBottomWall (FlowField&, int, int, int) override;
    void applyTopWall    (FlowField&, int, int, int) override;
    void applyFrontWall  (FlowField&, int, int, int) override;
    void applyBackWall   (FlowField&, int, int, int) override;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__
