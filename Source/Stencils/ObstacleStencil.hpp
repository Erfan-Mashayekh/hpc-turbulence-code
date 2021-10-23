#ifndef __STENCILS_OBSTACLE_STENCIL_HPP__
#define __STENCILS_OBSTACLE_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

namespace NSEOF {
namespace Stencils {

/** Compute all velocities on obstacle cells
 */
class ObstacleStencil : public FieldStencil<FlowField> {
public:
    ObstacleStencil(const Parameters& parameters);
    ~ObstacleStencil() override = default;

    void apply(FlowField& flowField, int i, int j) override;
    void apply(FlowField& flowField, int i, int j, int k) override;
};

} // namespace Stencils
} // namespace NSEOF

#endif
