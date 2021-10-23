#ifndef __INIT_TAYLOR_GREEN_FLOW_FIELD_STENCIL_HPP__
#define __INIT_TAYLOR_GREEN_FLOW_FIELD_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <cmath>

namespace NSEOF {
namespace Stencils {

/**
 * Stencil for initializing the taylor-green vortex flow in a periodic domain.
 */
class InitTaylorGreenFlowFieldStencil : public FieldStencil<FlowField> {
private:
    const FLOAT pi2_;
    const FLOAT* const domainSize_;

    FLOAT* initializeDomainSize(const Parameters& parameters) const;

    /** from the local grid coordinates i, j, k, computes the global coordinates of the current cell and initializes
     *  the velocity field correspondingly.
     */
    void computeGlobalCoordinates(FLOAT* coords, int i, int j, int k = 0) const;

public:
    InitTaylorGreenFlowFieldStencil(const Parameters& parameters);
    ~InitTaylorGreenFlowFieldStencil() override;

    void apply(FlowField& flowField, int i, int j) override;
    void apply(FlowField& flowField, int i, int j, int k) override;
};

} // namespace Stencils
} // namespace NSEOF

#endif // __INIT_TAYLOR_GREEN_FLOW_FIELD_STENCIL_HPP__
