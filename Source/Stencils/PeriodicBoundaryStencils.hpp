#ifndef __STENCILS_PERIODIC_BOUNDARY_STENCIL_HPP__
#define __STENCILS_PERIODIC_BOUNDARY_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

namespace NSEOF {
namespace Stencils {

/** Stencil to set periodic boundary conditions for velocity
 */
class PeriodicBoundaryVelocityStencil : public BoundaryStencil<FlowField> {
public:
    PeriodicBoundaryVelocityStencil(const Parameters& parameters);
    ~PeriodicBoundaryVelocityStencil() override = default;

    void applyLeftWall(FlowField& flowField, int i, int j) override;
    void applyRightWall(FlowField& flowField, int i, int j) override;
    void applyBottomWall(FlowField& flowField, int i, int j) override;
    void applyTopWall(FlowField& flowField, int i, int j) override;

    void applyLeftWall(FlowField& flowField, int i, int j, int k) override;
    void applyRightWall(FlowField& flowField, int i, int j, int k) override;
    void applyBottomWall(FlowField& flowField, int i, int j, int k) override;
    void applyTopWall(FlowField& flowField, int i, int j, int k) override;
    void applyFrontWall(FlowField& flowField, int i, int j, int k) override;
    void applyBackWall(FlowField& flowField, int i, int j, int k) override;
};

/** Stencil to set periodic boundary conditions for velocity for FGH. Since there are no operations
 * in FGH, this stencil does nothing.
 */
class PeriodicBoundaryFGHStencil: public BoundaryStencil<FlowField> {
public:
    PeriodicBoundaryFGHStencil(const Parameters& parameters);
    ~PeriodicBoundaryFGHStencil() override = default;

    void applyLeftWall(FlowField& flowField, int i, int j) override;
    void applyRightWall(FlowField& flowField, int i, int j) override;
    void applyBottomWall(FlowField& flowField, int i, int j) override;
    void applyTopWall(FlowField& flowField, int i, int j) override;

    void applyLeftWall(FlowField& flowField, int i, int j, int k) override;
    void applyRightWall(FlowField& flowField, int i, int j, int k) override;
    void applyBottomWall(FlowField& flowField, int i, int j, int k) override;
    void applyTopWall(FlowField& flowField, int i, int j, int k) override;
    void applyFrontWall(FlowField& flowField, int i, int j, int k) override;
    void applyBackWall(FlowField& flowField, int i, int j, int k) override;
};

} // namespace Stencils
} // namespace NSEOF

#endif // __STENCILS_PERIODIC_BOUNDARY_STENCIL_HPP__
