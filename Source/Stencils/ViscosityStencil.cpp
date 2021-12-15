#include "ViscosityStencil.hpp"
#include "StencilFunctions.hpp"


namespace NSEOF::Stencils {

FLOAT getReynoldsXPow(const Parameters& parameters) {
    const int turbulenceModel = parameters.turbulence.model;

    if (turbulenceModel == 1) { // Boundary layer thickness of a laminar flat plate
        return 0.5;
    } else if (turbulenceModel == 2) { // Boundary layer thickness of a turbulent flat plate
        return 0.2;
    }

    return 0.0;
}

FLOAT getBoundaryThicknessMultiplier(const Parameters& parameters) {
    const int turbulenceModel = parameters.turbulence.model;

    if (turbulenceModel == 1) { // Boundary layer thickness of a laminar flat plate
        return 4.910;
    } else if (turbulenceModel == 2) { // Boundary layer thickness of a turbulent flat plate
        return 0.382;
    }

    return 0.0;
}

ViscosityStencil::ViscosityStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters)
    , VISCOSITY_CONSTANT(1 / parameters.flow.Re)
    , REYNOLDS_X_POW(getReynoldsXPow(parameters))
    , BOUNDARY_THICKNESS_MULTIPLIER(getBoundaryThicknessMultiplier(parameters)) {}

FLOAT ViscosityStencil::calculateBoundaryThickness(int i, int j, int k) {
    if (x <= 0) {
        return 0.0;
    }

    const FLOAT x = parameters_.meshsize->getPosX(i, j, k);
    const FLOAT reynoldsX = std::min(U0 * x / VISCOSITY_CONSTANT, parameters_.flow.Re);

    return BOUNDARY_THICKNESS_MULTIPLIER * x / std::pow(reynoldsX, REYNOLDS_X_POW);
}

FLOAT ViscosityStencil::calculateMixingLength_(FlowField& flowField, int i, int j, int k = 0) {
    /**
     * Turbulence Model == 0: Prandtl mixing length is kappa times distance to the nearest wall
     * Turbulence Model == 1: Boundary layer thickness of a laminar flat plate
     * Turbulence Model == 2: Boundary layer thickness of a turbulent flat plate
     * Turbulence Model == 3: TODO (Extra): extract local boundary thickness from laminar reference case ???
     */
	const FLOAT kappaH = KAPPA * flowField.getDistance().getScalar(i, j, k);

    // Compute Prandtl mixing length
    if (parameters_.turbulence.model == 0) {
        return kappaH;
    } else if (parameters_.turbulence.model == 1 || parameters_.turbulence.model == 2) {
        const FLOAT boundary_thickness = calculateBoundaryThickness(i, j, k);
        return std::min(MIXING_LENGTH_MULTIPLIER * boundary_thickness, kappaH);
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
