#include "ViscosityStencil.hpp"
#include "StencilFunctions.hpp"


namespace NSEOF {
namespace Stencils {

ViscosityStencil::ViscosityStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters) {}

void ViscosityStencil::apply(FlowField& flowField, int i, int j) {
	FLOAT viscosity = 1/parameters_.flow.Re;
	// is that a right value for u_0?
	int u0 = 1;
	FLOAT kappa = 0.41;
	FLOAT mixing_length = 0.;
	FLOAT& eddy_viscosity = flowField.getEddyViscosity().getScalar(i,j);

	// compute Prandtl mixing length
	if(parameters_.turbulence.model == 0){
		// Prandtl mixing length is kappa times distance to nearest wall
		mixing_length = kappa * flowField.getDistance().getScalar(i,j);
		
	}else if(parameters_.turbulence.model ==1){
		// boundary layer thickness of a laminar flat plate
		FLOAT x = parameters_.meshsize->getPosX(i,j);
		//TODO: different calculation for nackward facing step?
		FLOAT Re_x = u0 * x/viscosity;
		FLOAT boundary_thickness = 4.91 * x / std::sqrt(Re_x);
		mixing_length = 0.09 * boundary_thickness;	

	}else if(parameters_.turbulence.model == 2){
		// boundary layer thickness of a turbulent flat plate
		FLOAT x = parameters_.meshsize->getPosX(i,j);
		//TODO: different calculation for nackward facing step?
		FLOAT Re_x = u0 * x/viscosity;
		FLOAT boundary_thickness = 0.382 * x / std::pow(Re_x, 0.2);
		mixing_length = 0.09 * boundary_thickness;	

	}else if(parameters_.turbulence.model == 3){
		//TODO: extract local boundary thickness from laminar reference case

	}else{ 
		std::cout << "invalid input for turbulence model" << std::endl;
	}

	//compute strain tensor
	loadLocalVelocity2D(flowField, localVelocity_, i, j);
	loadLocalMeshsize2D(parameters_, localMeshsize_, i, j);
	
	FLOAT strain_tensor_squared = computeStrainTensorSquared2D(localVelocity_, localMeshsize_);
	
	//compute eddy vicosity
	if(parameters_.turbulence.turb_viscosity == 0){
		eddy_viscosity = 0.0;
	}else{
		eddy_viscosity = std::pow(mixing_length,2) * std::sqrt(2 * strain_tensor_squared);
	}

}

void ViscosityStencil::apply(FlowField& flowField, int i, int j, int k) {

	FLOAT viscosity = 1/parameters_.flow.Re;
	int u0 = 1;
	FLOAT kappa = 0.41;
	FLOAT mixing_length = 0.;
	FLOAT& eddy_viscosity = flowField.getEddyViscosity().getScalar(i,j,k);

	// compute Prandtl mixing length
	if(parameters_.turbulence.model == 0){
		//Prandtl mixing length is kappa times distance to nearest wall
		mixing_length = kappa * flowField.getDistance().getScalar(i,j,k);
		
	}else if(parameters_.turbulence.model ==1){
		// boundary layer thickness of a laminar flat plate
		FLOAT x = parameters_.meshsize->getPosX(i,j,k);
		FLOAT Re_x = u0 * x/viscosity;
		FLOAT boundary_thickness = 4.91 * x / std::sqrt(Re_x);
		mixing_length = 0.09 * boundary_thickness;	

	}else if(parameters_.turbulence.model == 2){
		// boundary layer thickness of a turbulent flat plate
		FLOAT x = parameters_.meshsize->getPosX(i,j,k);
		FLOAT Re_x = u0 * x/viscosity;
		FLOAT boundary_thickness = 0.382 * x / std::pow(Re_x, 0.2);
		mixing_length = 0.09 * boundary_thickness;	

	}else if(parameters_.turbulence.model == 3){
		//TODO: extract local boundary thickness from laminar reference case

	}else{ 
		std::cout << "invalid input for turbulence model" << std::endl;
	}

	//compute strain tensor
	loadLocalVelocity3D(flowField, localVelocity_, i, j, k);
	loadLocalMeshsize3D(parameters_, localMeshsize_, i, j, k);
	
	FLOAT strain_tensor_squared = computeStrainTensorSquared3D(localVelocity_, localMeshsize_);
	
	//compute eddy vicosity
	if(parameters_.turbulence.turb_viscosity == 0){
		eddy_viscosity = 0.0;
	}else{
		eddy_viscosity = std::pow(mixing_length,2) * std::sqrt(2 * strain_tensor_squared);
	}
}

} // namespace Stencils
} // namespace NSEOF
