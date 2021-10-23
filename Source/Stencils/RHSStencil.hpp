#ifndef __STENCILS_RHS_STENCIL_HPP__
#define __STENCILS_RHS_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

namespace NSEOF {
namespace Stencils {

/** Field stencil to compute the right hand side of the pressure equation.
 */
class RHSStencil : public FieldStencil<FlowField> {
public:
    RHSStencil(const Parameters& parameters);
    ~RHSStencil() override = default;

    void apply(FlowField& flowField, int i, int j) override;
    void apply(FlowField& flowField, int i, int j, int k) override;
};

} // namespace Stencils
} // namespace NSEOF

#endif // __STENCILS_RHS_STENCIL_HPP__
