#ifndef __STENCILS_FGH_STENCIL_HPP__
#define __STENCILS_FGH_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include "StencilFunctions.hpp"

#define VALUES_DIMENSION 27 * 3

namespace NSEOF::Stencils {

class FGHStencil : public FieldStencil<FlowField> {
private:
    // A local velocity variable that will be used to approximate derivatives.
    // Size matches 3D case, but can be used for 2D as well.
    FLOAT localVelocity_[VALUES_DIMENSION]{};
    FLOAT localMeshsize_[VALUES_DIMENSION]{};

protected:
    virtual void computeValues_(FlowField&, int, int);
    virtual void computeValues_(FlowField&, int, int, int, int);

    [[nodiscard]] FLOAT* getLocalVelocity_();
    [[nodiscard]] FLOAT* getLocalMeshsize_();

public:
    explicit FGHStencil(const Parameters& parameters);
    ~FGHStencil() override = default;

    void apply(FlowField&, int, int) override;
    void apply(FlowField&, int, int, int) override;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_FGH_STENCIL_HPP__
