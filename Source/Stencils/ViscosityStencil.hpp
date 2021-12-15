#ifndef __STENCILS_VISCOSITY_STENCIL_HPP__
#define __STENCILS_VISCOSITY_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

#define VALUES_DIMENSION 27 * 3

namespace NSEOF::Stencils {

/**
 * Stencil to compute the turbulence/eddy viscosity for a chosen turbulence model
 */
class ViscosityStencil : public FieldStencil<FlowField> {
private:
    const FLOAT VISCOSITY_CONSTANT;
    const FLOAT U0 = 1;
    const FLOAT KAPPA = 0.41;
    const FLOAT REYNOLDS_X_POW;
    const FLOAT BOUNDARY_THICKNESS_MULTIPLIER;
    const FLOAT MIXING_LENGTH_MULTIPLIER = 0.09;

	// A local velocity variable that will be used to approximate the derivative.
    // Size matches 3D cases but can be used for 2D as well.
	FLOAT localVelocity_[VALUES_DIMENSION];
    FLOAT localMeshsize_[VALUES_DIMENSION];

    FLOAT calculateBoundaryThickness_();
    FLOAT calculateMixingLength_(FlowField&, int, int, int);
public:
    explicit ViscosityStencil(const Parameters& parameters);
    ~ViscosityStencil() override = default;

    void apply(FlowField & flowField, int i, int j) override;
    void apply(FlowField & flowField, int i, int j, int k) override;
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VISCOSITY_STENCIL_HPP__
