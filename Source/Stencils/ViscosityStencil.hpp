#ifndef __STENCILS_VISCOSITY_STENCIL_HPP__
#define __STENCILS_VISCOSITY_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

namespace NSEOF {
namespace Stencils {

/** Stencil to compute the turbulence/eddy viscosity for a chosen turbulence model.
 */
class ViscosityStencil : public FieldStencil<FlowField> {
public:
    ViscosityStencil(const Parameters& parameters);
    ~ViscosityStencil() override = default;

    void apply(FlowField & flowField, int i, int j) override;
    void apply(FlowField & flowField, int i, int j, int k) override;
};

} // namespace Stencils
} // namespace NSEOF

#endif // __STENCILS_VISCOSITY_STENCIL_HPP__
