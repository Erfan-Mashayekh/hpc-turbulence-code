#ifndef __STENCILS_VTK_STENCIL_HPP__
#define __STENCILS_VTK_STENCIL_HPP__

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

struct PositionIdx {

public:
    int i, j, k;

    PositionIdx(int, int, int);
};

class VTKStencil : public FieldStencil<FlowField> {

private:
    std::vector<PositionIdx> positionsIdxList_;
    ScalarField pressure_;
    VectorField velocity_;

    void writePositions_(FILE*);
    void writePressures_(FILE*);
    void writeVelocities_(FILE*);

public:
    explicit VTKStencil(const Parameters &parameters);
    ~VTKStencil() override;

    void apply(FlowField& flowField, int i, int j, int k) override;
    void apply(FlowField& flowField, int i, int j) override;

    void write(int timeStep);
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VTK_STENCIL_HPP__
