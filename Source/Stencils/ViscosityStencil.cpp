#include "ViscosityStencil.hpp"

namespace NSEOF {
namespace Stencils {

ViscosityStencil::ViscosityStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters) {}

void ViscosityStencil::apply(FlowField& flowField, int i, int j) {



}

void ViscosityStencil::apply(FlowField& flowField, int i, int j, int k) {

}

} // namespace Stencils
} // namespace NSEOF
