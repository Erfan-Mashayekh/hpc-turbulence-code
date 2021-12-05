#include "ViscosityStencil.hpp"

namespace NSEOF {
namespace Stencils {

ViscosityStencil::ViscosityStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters) {}

void ViscosityStencil::apply(FlowField& flowField, int i, int j) {
	// TODO: change for input parameter
	int model = 0;
	FLOAT viscosiy = 1/parameters_.flow.Re;
	//TODO: is that a right value for u_0?
	int u0 = 0;
	FLOAT kappa = 0.41;
	FLOAT mixing_length = 0.;
	ScalarField& eddy_viscosity = flowField.getEddyViscosity().getScalar(i,j);

	// compute Prandtl mixing length
//TODO: switch for turbulence model
	if(model == 0){
		//TODO: Prandtl mixing length is kappa times distance to nearest wall
		mixing_length = kappa * flowField.getDistance().getScalar(i,j);
		
	}elseif(model ==1){
		// boundary layer thickness of a laminar flat plate
		// TODO: x? everytime? what abou cavity?
		FLOAT x = parameters_.meshsize->getPosX(i,j);
		//TODO: HowTo calculate Re(x)? how to get u0?
		FLOAT Re_x = u0 * x/viscosity;
		FLOAT boundary_thickness = 4.91 * x / std::sqrt(Re_x);
		mixing_length = 0.09 * boundary_thickness;	

	}elseif(model == 2){
		// boundary layer thickness of a turbulent flat plate
		// TODO: x? everytime? what abou cavity?
		FLOAT x = parameters_.meshsize->getPosX(i,j);
		//TODO: HowTo calculate Re(x)? how to get u0?
		FLOAT Re_x = u0 * x/viscosity;
		FLOAT boundary_thickness = 0.382 * x / std::pow(Re_x, 0.2);
		mixing_length = 0.09 * boundary_thickness;	

	}elseif(model == 3){
		//TODO: extract local boundary thickness from laminar reference case

	}else{ 
		std::cout << "invalid input for turbulence model" << std::endl;
	}

	//TODO: compute strain tensor
	loadLocalVelocity2D(flowField, localVelocity_, i, j);
	loadLocalMeshsize2D(parameters_, localMeshsize_, i, j);
	
	FLOAT strain_tensor_squared = computeStrainTensorSquare2D(localVelocity_, localMeshsize_);
	//compute eddy vicosity
	eddy_viscosity = std::pow(mixing_length,2) * std::sqrt(2 * strain_tensor_squared);

}

void ViscosityStencil::apply(FlowField& flowField, int i, int j, int k) {

	// TODO: change for input parameter
	int model = 0;
	FLOAT viscosiy = 1/parameters_.flow.Re;
	//TODO: is that a right value for u_0?
	int u0 = 0;
	FLOAT kappa = 0.41;
	FLOAT mixing_length = 0.;
	ScalarField& eddy_viscosity = flowField.getEddyViscosity().getScalar(i,j,k);

	// compute Prandtl mixing length
//TODO: switch for turbulence model
	if(model == 0){
		//TODO: Prandtl mixing length is kappa times distance to nearest wall
		mixing_length = kappa * flowField.getDistance().getScalar(i,j,k);
		
	}elseif(model ==1){
		// boundary layer thickness of a laminar flat plate
		// TODO: x? everytime? what abou cavity?
		FLOAT x = parameters_.meshsize->getPosX(i,j,k);
		//TODO: HowTo calculate Re(x)? how to get u0?
		FLOAT Re_x = u0 * x/viscosity;
		FLOAT boundary_thickness = 4.91 * x / std::sqrt(Re_x);
		mixing_length = 0.09 * boundary_thickness;	

	}elseif(model == 2){
		// boundary layer thickness of a turbulent flat plate
		// TODO: x? everytime? what abou cavity?
		FLOAT x = parameters_.meshsize->getPosX(i,j,k);
		//TODO: HowTo calculate Re(x)? how to get u0?
		FLOAT Re_x = u0 * x/viscosity;
		FLOAT boundary_thickness = 0.382 * x / std::pow(Re_x, 0.2);
		mixing_length = 0.09 * boundary_thickness;	

	}elseif(model == 3){
		//TODO: extract local boundary thickness from laminar reference case

	}else{ 
		std::cout << "invalid input for turbulence model" << std::endl;
	}

	//TODO: compute strain tensor
	loadLocalVelocity3D(flowField, localVelocity_, i, j, k);
	loadLocalMeshsize3D(parameters_, localMeshsize_, i, j, k);
	
	FLOAT strain_tensor_squared = computeStrainTensorSquare3D(localVelocity_, localMeshsize_);
	//compute eddy vicosity
	eddy_viscosity = std::pow(mixing_length,2) * std::sqrt(2 * strain_tensor_squared);
}

} // namespace Stencils
} // namespace NSEOF
