#ifndef __STENCILS_VTK_TURBULENCE_STENCIL_HPP__
#define __STENCILS_VTK_TURBULENCE_STENCIL_HPP__

#include "VTKStencil.hpp"

namespace NSEOF::Stencils {

/** TODO WS1: Stencil for writing VTK files for Turbulence Model
 *
 * When iterated with, creates a VTK file.
 */

class VTKTurbulenceStencil : public VTKStencil {

private:
    ScalarField eddyViscosity_;
    ScalarField distanceToWall_;

    void writeEddyViscosities_(FILE*);
    void writeDistanceToWalls_(FILE*);

protected:
    void writeValues(FILE*) override;

public:
    explicit VTKTurbulenceStencil(const Parameters&, int, int, int);
    ~VTKTurbulenceStencil() override = default;

    void apply(FlowField&, int, int, int) override;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VTK_T_STENCIL_HPP__
