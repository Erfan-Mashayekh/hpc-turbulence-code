#ifndef __STENCILS_BF_STEP_INIT_STENCIL_HPP__
#define __STENCILS_BF_STEP_INIT_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Definitions.hpp"

namespace NSEOF {
namespace Stencils {

/** Initializes the backward facing step scenario, i.e. sets the flag field.
 */
class BFStepInitStencil : public FieldStencil<FlowField> {
private:
    const FLOAT xLimit_; //! Size of step in x-direction
    const FLOAT yLimit_; //! Same as for x

public:
    BFStepInitStencil(const Parameters& parameters);
    ~BFStepInitStencil() override = default;

    void apply(FlowField& flowField, int i, int j) override;
    void apply(FlowField& flowField, int i, int j, int k) override;
};

} // namespace Stencils
} // namespace NSEOF

#endif // __STENCILS_BF_STEP_INIT_STENCIL_HPP__
