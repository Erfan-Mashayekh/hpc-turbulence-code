#ifndef __STENCILS_MIN_TIMESTEP_STENCIL_HPP__
#define __STENCILS_MIN_TIMESTEP_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

namespace NSEOF {
namespace Stencils {

//This class comutes the minimum diffusive timestep for the turbulent cases
class MinTimeStepStencil : public FieldStencil<FlowField> {
private:
    FLOAT diffusiveTimeStep_; //! Stores the minimum diffusive time step
    
    //finds the current cell timestep and changes the diffusiveTimeStep_ to the current timestep, if the current one is smaller
    void cellMinValue(FlowField& flowField, int i, int j);

    void cellMinValue(FlowField& flowField, int i, int j, int k);

public:
    MinTimeStepStencil(const Parameters& parameters);
    ~MinTimeStepStencil() override = default;

    void apply(FlowField& flowField, int i, int j) override;
    void apply(FlowField& flowField, int i, int j, int k) override;

    /** Resets the minimum value to zero before computing the timestep.
     */
    void reset();

    // Returns the minimum timestep over the whole geometry,
    FLOAT getDiffusiveTimeStep() const;
};

} // namespace Stencils
} // namespace NSEOF

#endif // __STENCILS_MIN_TIMESTEP_STENCIL_HPP__
