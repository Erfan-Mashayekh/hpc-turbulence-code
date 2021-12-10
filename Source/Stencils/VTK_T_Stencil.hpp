#ifndef __STENCILS_V_TURB_STENCIL_HPP__
#define __STENCILS_V_TURB_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace NSEOF::Stencils {

/** TODO WS1: Stencil for writting VTK files
 *
 * When iterated with, creates a VTK file.
 */

struct CellIndexx {

public:
    int i, j, k;

    CellIndexx(int, int, int);
};

class VTK_T_Stencil : public FieldStencil<FlowField> {

private:
    std::vector<CellIndexx> cellIndices_;
    ScalarField pressure_;
    VectorField velocity_;
    ScalarField eddyViscosity_;
    ScalarField distanceToWall_;

    void writePositions_(FILE*);
    void writePressures_(FILE*);
    void writeVelocities_(FILE*);
    void writeEddyViscosity_(FILE*);
    void writeDistanceToWall_(FILE*);

public:
    explicit VTK_T_Stencil(const Parameters&, int, int, int);
    ~VTK_T_Stencil() override;

    void apply(FlowField&, int, int, int) override;
    void apply(FlowField&, int, int) override;

    void write(int);
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VTK_T_STENCIL_HPP__
