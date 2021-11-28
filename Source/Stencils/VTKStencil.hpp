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
class VTKStencil : public FieldStencil<FlowField> {

private:
    int firstCornerIdx[3] = { -1, -1, -1 };
    std::vector<FLOAT> pressures_;
    std::vector<FLOAT*> velocities_;

    void writePositions_(FILE*);
    void writePressures_(FILE*);
    void writeVelocities_(FILE*);

public:
    explicit VTKStencil(const Parameters&);
    ~VTKStencil() override;

    void apply(FlowField&, int, int, int) override;
    void apply(FlowField&, int, int) override;

    void write(int);
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VTK_STENCIL_HPP__
