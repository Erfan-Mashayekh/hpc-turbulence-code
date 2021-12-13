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

/** TODO WS1: Stencil for writing VTK files
 *
 * When iterated with, creates a VTK file.
 */

struct CellIndex {
public:
    int i, j, k;

    CellIndex(int, int, int);
};

class VTKStencil : public FieldStencil<FlowField> {
private:
    std::vector<CellIndex> cellIndices_;
    ScalarField pressure_;
    VectorField velocity_;

    void writePositions_(FILE*);
    void writePressures_(FILE*);
    void writeVelocities_(FILE*);

protected:
    virtual void writeValues(FILE*);

    [[nodiscard]] const std::vector<CellIndex>& getCellIndices() const;

public:
    explicit VTKStencil(const Parameters&, int, int, int);
    ~VTKStencil() override;

    void apply(FlowField&, int, int, int) override;
    void apply(FlowField&, int, int) override;

    void write(int);
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VTK_STENCIL_HPP__
