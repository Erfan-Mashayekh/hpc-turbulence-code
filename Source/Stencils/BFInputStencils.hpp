#ifndef __STENCILS_BF_INPUT_STENCILS_HPP__
#define __STENCILS_BF_INPUT_STENCILS_HPP__

#include "Stencil.hpp"
#include "Parameters.hpp"
#include "FlowField.hpp"

namespace NSEOF {
namespace Stencils {

/**
 * A stencil to set the input velocity in channel flows. Only implements the applyLeftWall(...) methods.
 */
class BFInputVelocityStencil : public BoundaryStencil<FlowField> {
private:
    FLOAT stepSize_; //! Fixes the size of the step. If zero, is just channel flow.

public:
    BFInputVelocityStencil(const Parameters& parameters);
    ~BFInputVelocityStencil() override = default;

    void applyLeftWall   (FlowField& flowField, int i, int j) override;
    void applyRightWall  (FlowField& flowField, int i, int j) override;
    void applyBottomWall (FlowField& flowField, int i, int j) override;
    void applyTopWall    (FlowField& flowField, int i, int j) override;

    void applyLeftWall   (FlowField& flowField, int i, int j, int k) override;
    void applyRightWall  (FlowField& flowField, int i, int j, int k) override;
    void applyBottomWall (FlowField& flowField, int i, int j, int k) override;
    void applyTopWall    (FlowField& flowField, int i, int j, int k) override;
    void applyFrontWall  (FlowField& flowField, int i, int j, int k) override;
    void applyBackWall   (FlowField& flowField, int i, int j, int k) override;
};

/** FGH stencil which corresponds to one-sided Dirichlet conditions (i.e. the channel flow profile).
 *  Only implements the applyLeftWall(...) methods.
 */
class BFInputFGHStencil : public BoundaryStencil<FlowField> {
private:
    const FLOAT stepSize_; //! Fixes the size of the step. If zero, is just channel flow.

public:
    BFInputFGHStencil(const Parameters& parameters);
    ~BFInputFGHStencil() override = default;

    void applyLeftWall   (FlowField& flowField, int i, int j) override;
    void applyRightWall  (FlowField& flowField, int i, int j) override;
    void applyBottomWall (FlowField& flowField, int i, int j) override;
    void applyTopWall    (FlowField& flowField, int i, int j) override;

    void applyLeftWall   (FlowField& flowField, int i, int j, int k) override;
    void applyRightWall  (FlowField& flowField, int i, int j, int k) override;
    void applyBottomWall (FlowField& flowField, int i, int j, int k) override;
    void applyTopWall    (FlowField& flowField, int i, int j, int k) override;
    void applyFrontWall  (FlowField& flowField, int i, int j, int k) override;
    void applyBackWall   (FlowField& flowField, int i, int j, int k) override;
};

} // namespace Stencils
} // namespace NSEOF

#endif // __STENCILS_BF_INPUT_STENCILS_HPP__
