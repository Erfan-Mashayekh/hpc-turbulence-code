#ifndef __STENCILS_FGH_STENCIL_HPP__
#define __STENCILS_FGH_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include "StencilFunctions.hpp"

namespace NSEOF::Stencils {

class FGHStencil : public FieldStencil<FlowField> {
private:
    // A local velocity variable that will be used to approximate derivatives. Size matches 3D
    // case, but can be used for 2D as well.
    FLOAT localVelocity_[27 * 3]{};
    FLOAT localMeshsize_[27 * 3]{};
    FLOAT localViscosity_[27 * 3]{};

public:
    explicit FGHStencil(const Parameters& parameters);
    ~FGHStencil() override = default;

    void apply(FlowField& flowField, int i, int j) override;
    void apply(FlowField& flowField, int i, int j, int k) override;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_FGH_STENCIL_HPP__
