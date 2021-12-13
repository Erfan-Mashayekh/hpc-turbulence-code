#ifndef __STENCILS_MIN_TIME_STEP_STENCIL_HPP__
#define __STENCILS_MIN_TIME_STEP_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

#include <algorithm>
#include <math.h>

namespace NSEOF::Stencils {

/**
 * This class commutes the minimum diffusive timestep for the turbulent cases
 */
class MinTimeStepStencil : public FieldStencil<FlowField> {
private:
    FLOAT diffusiveTimeStep_; //! Stores the minimum diffusive timestep

public:
    explicit MinTimeStepStencil(const Parameters& parameters);
    ~MinTimeStepStencil() override = default;

    void apply(FlowField& flowField, int i, int j) override;
    void apply(FlowField& flowField, int i, int j, int k) override;

    // Resets the minimum value to zero before computing the timestep.
    void reset();

    // Returns the minimum timestep over the whole geometry,
    [[nodiscard]] FLOAT getDiffusiveTimeStep() const;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_MIN_TIME_STEP_STENCIL_HPP__
