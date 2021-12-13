#ifndef __STENCILS_TURBULENT_VTK_STENCIL_HPP__
#define __STENCILS_TURBULENT_VTK_STENCIL_HPP__

#include "VTKStencil.hpp"

namespace NSEOF::Stencils {

/** TODO WS1: Stencil for writing VTK files for Turbulence Model
 *
 * When iterated with, creates a VTK file.
 */

class TurbulentVTKStencil : public VTKStencil {
private:
    ScalarField eddyViscosity_;
    ScalarField distanceToWall_;

    void writeEddyViscosities_(FILE*);
    void writeDistanceToWalls_(FILE*);

protected:
    void writeValues(FILE*) override;

public:
    explicit TurbulentVTKStencil(const Parameters&, int, int, int);
    ~TurbulentVTKStencil() override = default;

    void apply(FlowField&, int, int, int) override;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_TURBULENT_VTK_STENCIL_HPP__
