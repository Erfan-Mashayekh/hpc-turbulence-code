#include "ViscosityStencil.hpp"

namespace NSEOF {
namespace Stencils {

ViscosityStencil::ViscosityStencil(const Parameters& parameters)
    : FieldStencil<FlowField>(parameters) {}

void ViscosityStencil::apply(FlowField& flowField, int i, int j) {
	// TODO: change for input parameter
	int model = 0;
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
		// TODO: x, Re_x
		int x = 0.;
		FLOAT Re_x = 0.;
		FLOAT boundary_thickness = 4.91 * x / std::sqrt(Re_x);
		mixing_length = 0.09 * boundary_thickness;	

	}elseif(model == 2){
		// boundary layer thickness of a turbulent flat plate
		// TODO: x, Re_x
		int x = 0.;
		FLOAT Re_x = 0.;
		FLOAT boundary_thickness = 0.382 * x / std::pow(Re_x, 0.2);
		mixing_length = 0.09 * boundary_thickness;	

	}elseif(model == 3){
		//TODO: extract local boundary thickness from laminar reference case

	}else{ 
		std::cout << "invalid input for turbulence model" << std::endl;
	}

	//TODO: compute strain tensor
	FLOAT strain_tensor = 1.;
	//compute eddy vicosity
	eddy_viscosity = std::pow(mixing_length,2) * std::sqrt(2 * strain_tensor * strain_tensor);

}

void ViscosityStencil::apply(FlowField& flowField, int i, int j, int k) {

}

} // namespace Stencils
} // namespace NSEOF
