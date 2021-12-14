#include "ViscosityStencil.hpp"
#include "StencilFunctions.hpp"


namespace NSEOF::Stencils {

FLOAT getReynoldsXPow(const Parameters& parameters) {
    const int turbulenceModel = parameters.turbulence.model;

    if (turbulenceModel == 1) {
        return 0.5;
    } else if (turbulenceModel == 2) {
        return 0.2;
    }

    return 0.0; // TODO: (turbulentModel == 3) extract local boundary thickness from laminar reference case
}

FLOAT getBoundaryThicknessMultiplier(const Parameters& parameters) {
    const int turbulenceModel = parameters.turbulence.model;

    if (turbulenceModel == 1) {
        return 4.910;
    } else if (turbulenceModel == 2) {
        return 0.382;
    }

    return 0.0; // TODO: (turbulentModel == 3) extract local boundary thickness from laminar reference case
}

ViscosityStencil::ViscosityStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters)
    , VISCOSITY_CONSTANT(1 / parameters.flow.Re)
    , REYNOLDS_X_POW(getReynoldsXPow(parameters))
    , BOUNDARY_THICKNESS_MULTIPLIER(getBoundaryThicknessMultiplier(parameters)) {}

FLOAT ViscosityStencil::calculateMixingLength_(FlowField& flowField, int i, int j, int k = 0) {
    /**
     * Turbulence Model == 0: Prandtl mixing length is kappa times distance to the nearest wall
     * Turbulence Model == 1: Boundary layer thickness of a laminar flat plate
     * Turbulence Model == 2: Boundary layer thickness of a turbulent flat plate
     * Turbulence Model == 3: TODO ???
     */

    // Compute Prandtl mixing length
    if (parameters_.turbulence.model == 0) {
        return KAPPA * flowField.getDistance().getScalar(i, j, k);
    } else if (parameters_.turbulence.model == 1 || parameters_.turbulence.model == 2) {
        FLOAT x = parameters_.meshsize->getPosX(i, j, k);

        // TODO: different calculation for backward facing step?
        FLOAT reynoldsX = U0 * x / VISCOSITY_CONSTANT;
        FLOAT boundary_thickness = BOUNDARY_THICKNESS_MULTIPLIER * x / std::pow(reynoldsX, REYNOLDS_X_POW);

        return MIXING_LENGTH_MULTIPLIER * boundary_thickness;
    } else if (parameters_.turbulence.model == 3) {
        // TODO: extract local boundary thickness from laminar reference case. Only 2 model necessary..
    }

    std::cerr << "Invalid input for turbulence model in viscosity!" << std::endl;
    return 0.0;
}

void ViscosityStencil::apply(FlowField& flowField, int i, int j) {
    FLOAT& eddyViscosity = flowField.getEddyViscosity().getScalar(i, j);

    // Compute strain tensor
    loadLocalVelocity2D(flowField, localVelocity_, i, j);
    loadLocalMeshsize2D(parameters_, localMeshsize_, i, j);

    FLOAT strain_tensor_squared = computeStrainTensorSquared2D(localVelocity_, localMeshsize_);

    // Compute eddy viscosity
    const FLOAT mixingLength = calculateMixingLength_(flowField, i, j);
    eddyViscosity = std::pow(mixingLength, 2) * std::sqrt(2 * strain_tensor_squared);
}

void ViscosityStencil::apply(FlowField& flowField, int i, int j, int k) {
    FLOAT& eddyViscosity = flowField.getEddyViscosity().getScalar(i, j, k);

    // Compute strain tensor
    loadLocalVelocity3D(flowField, localVelocity_, i, j, k);
    loadLocalMeshsize3D(parameters_, localMeshsize_, i, j, k);

    FLOAT strain_tensor_squared = computeStrainTensorSquared3D(localVelocity_, localMeshsize_);

    // Compute eddy viscosity
    const FLOAT mixingLength = calculateMixingLength_(flowField, i, j, k);
    eddyViscosity = std::pow(mixingLength, 2) * std::sqrt(2 * strain_tensor_squared);
}

} // namespace NSEOF::Stencils
