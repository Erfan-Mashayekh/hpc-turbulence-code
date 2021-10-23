#ifndef __STENCILS_VTK_STENCIL_HPP__
#define __STENCILS_VTK_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <string>
#include <fstream>
#include <sstream>

namespace NSEOF {
namespace Stencils {

/** TODO WS1: Stencil for writting VTK files
 *
 * When iterated with, creates a VTK file.
 */
class VTKStencil : public FieldStencil<FlowField> {

public:
    VTKStencil(const Parameters& parameters);
    ~VTKStencil() override = default;

    void apply(FlowField& flowField, int i, int j) override;
    void apply(FlowField& flowField, int i, int j, int k) override;

    void write(FlowField& flowField, int timeStep);
};

} // namespace Stencils
} // namespace NSEOF

#endif // __STENCILS_VTK_STENCIL_HPP__
