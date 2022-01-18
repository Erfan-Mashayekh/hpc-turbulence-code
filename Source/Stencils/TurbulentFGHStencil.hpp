#ifndef __STENCILS_TURBULENT_FGH_STENCIL_HPP__
#define __STENCILS_TURBULENT_FGH_STENCIL_HPP__

#include "FGHStencil.hpp"

namespace NSEOF::Stencils {

class TurbulentFGHStencil : public FGHStencil {
private:
    FLOAT localViscosity_[VALUES_DIMENSION]{};

protected:
    void computeValues_(FlowField&, int, int) override;
    void computeValues_(FlowField&, int, int, int, int) override;

public:
    explicit TurbulentFGHStencil(const Parameters& parameters);
    ~TurbulentFGHStencil() override = default;
};

} // namespace NSEOF::Stencils

#endif //__STENCILS_TURBULENT_FGH_STENCIL_HPP__
