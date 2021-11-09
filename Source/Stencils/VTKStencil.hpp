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
    int* firstCornerInd_;
    FLOAT* firstCornerPos_;

    std::vector<FLOAT> pressures_;
    std::vector<FLOAT*> velocities_;

    void setFirstCorner_(int, int, int);

    void writePositions_(FILE *filePtr);
    void writePressures_(FILE*);
    void writeVelocities_(FILE*);

public:
    VTKStencil(const Parameters& parameters);
    ~VTKStencil() override = default;

    void apply(FlowField& flowField, int i, int j, int k) override;
    void apply(FlowField& flowField, int i, int j) override;

    void write(int timeStep);
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VTK_STENCIL_HPP__
