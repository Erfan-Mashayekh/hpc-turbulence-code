#ifndef __STENCILS_VELOCITY_STENCIL_HPP__
#define __STENCILS_VELOCITY_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

namespace NSEOF::Stencils {

/** Stencil to compute the velocity once the pressure has been found.
 */
class VelocityStencil : public FieldStencil<FlowField> {
public:
    explicit VelocityStencil(const Parameters& parameters);
    ~VelocityStencil() override = default;

    void apply(FlowField & flowField, int i, int j, int k) override;
    void apply(FlowField & flowField, int i, int j) override;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VELOCITY_STENCIL_HPP__
