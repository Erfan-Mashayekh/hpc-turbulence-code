#include "Simulation.hpp"

#include "Stencils/VTKStencil.hpp"
#include "Stencils/VTK_T_Stencil.hpp"

#include "Solvers/SORSolver.hpp"
#include "Solvers/PetscSolver.hpp"

namespace NSEOF {

Simulation::Simulation(Parameters& parameters, FlowField& flowField)
    : parameters_(parameters)
    , flowField_(flowField)
    , maxUStencil_(parameters)
    , maxUFieldIterator_(flowField_, parameters, maxUStencil_)
    , maxUBoundaryIterator_(flowField_, parameters, maxUStencil_)
    , globalBoundaryFactory_(parameters)
    , wallVelocityIterator_(globalBoundaryFactory_.getGlobalBoundaryVelocityIterator(flowField_))
    , wallFGHIterator_(globalBoundaryFactory_.getGlobalBoundaryFGHIterator(flowField_))
    , fghStencil_(parameters)
    , fghIterator_(flowField_, parameters, fghStencil_)
    , rhsStencil_(parameters)
    , rhsIterator_(flowField_, parameters, rhsStencil_)
    , velocityStencil_(parameters)
    , obstacleStencil_(parameters)
    , velocityIterator_(flowField_, parameters, velocityStencil_)
    , obstacleIterator_(flowField_, parameters, obstacleStencil_)
    , viscosityStencil_(parameters)
    , viscosityIterator_(flowField_, parameters, viscosityStencil_)
    , minTimeStepStencil_(parameters)
    , minTimeStepIterator_(flowField_, parameters, minTimeStepStencil_)
#ifdef BUILD_WITH_PETSC
    , solver_(std::make_unique<Solvers::PetscSolver>(flowField_, parameters))
#else
    , solver_(std::make_unique<Solvers::SORSolver>(flowField_, parameters))
#endif
    {}

void Simulation::initializeFlowField() {
    if (parameters_.simulation.scenario == "taylor-green") {
        // Currently, a particular initialization is only required for the taylor-green vortex.
        Stencils::InitTaylorGreenFlowFieldStencil stencil(parameters_);
        FieldIterator<FlowField> iterator(flowField_, parameters_, stencil);
        iterator.iterate();
    } else if (parameters_.simulation.scenario == "channel") {
        Stencils::BFStepInitStencil stencil(parameters_);
        FieldIterator<FlowField> iterator(flowField_, parameters_, stencil, 0, 1);
        iterator.iterate();
        wallVelocityIterator_.iterate();
    } else if (parameters_.simulation.scenario == "pressure-channel") {
        // Set pressure boundaries here for left wall
        const FLOAT value = parameters_.walls.scalarLeft;
        ScalarField& rhs = flowField_.getRHS();

        if (parameters_.geometry.dim == 2) {
            const int sizey = flowField_.getNy();
            for (int i = 0; i < sizey + 3; i++) {
                rhs.getScalar(0, i) = value;
            }
        } else {
            const int sizey = flowField_.getNy();
            const int sizez = flowField_.getNz();
            for (int i = 0; i < sizey + 3; i++) {
                for (int j = 0; j < sizez + 3; j++) {
                    rhs.getScalar(0, i, j) = value;
                }
            }
        }

        // Do same procedure for domain flagging as for regular channel
        Stencils::BFStepInitStencil stencil(parameters_);
        FieldIterator<FlowField> iterator(flowField_, parameters_, stencil, 0, 1);
        iterator.iterate();
    }

    solver_->reInitMatrix();
    
    //ws2: solve and store the nearest distance calculation
    Simulation::distanceNearestWall();
}

void Simulation::solveTimestep() {
    // Determine and set max. timestep which is allowed in this simulation
    setTimeStep();
    // Compute eddy viscosity
    viscosityIterator_.iterate();
    // Compute FGH
    fghIterator_.iterate();
    // Set global boundary values
    wallFGHIterator_.iterate();
    // Compute the right hand side (RHS)
    rhsIterator_.iterate();
    // Solve for pressure 
    solver_->solve();
    // TODO WS2: communicate pressure values
    // Compute velocity
    velocityIterator_.iterate();
    obstacleIterator_.iterate();
    // TODO WS2: communicate velocity values
    // Iterate for velocities on the boundary
    wallVelocityIterator_.iterate();
}

void Simulation::plotVTK(int timeStep) {
    // TODO WS1: create VTKStencil and respective iterator; iterate stencil
    //           over flowField_ and write flow field information to VTK file.
    if(parameters_.turbulence.on == 1){
	    Stencils::VTK_T_Stencil vtkStencil_(parameters_, flowField_.getCellsX(), flowField_.getCellsY(), flowField_.getCellsZ());
	    FieldIterator<FlowField> vtkIterator_(flowField_, parameters_, vtkStencil_,
                                          parameters_.vtk.whiteRegionLowOffset, parameters_.vtk.whiteRegionHighOffset);
	    
	    vtkIterator_.iterate();
	    vtkStencil_.write(timeStep);
    
    }else{
	    Stencils::VTKStencil vtkStencil_(parameters_, flowField_.getCellsX(), flowField_.getCellsY(), flowField_.getCellsZ());
	    FieldIterator<FlowField> vtkIterator_(flowField_, parameters_, vtkStencil_,
                                          parameters_.vtk.whiteRegionLowOffset, parameters_.vtk.whiteRegionHighOffset);
	    
	    vtkIterator_.iterate();
	    vtkStencil_.write(timeStep);
    }
}

void Simulation::setTimeStep() {

	FLOAT localMin, globalMin;
	ASSERTION(parameters_.geometry.dim == 2 || parameters_.geometry.dim == 3);
	
	// Determine maximum velocity
	maxUStencil_.reset();
	maxUFieldIterator_.iterate();
	maxUBoundaryIterator_.iterate();

	if(parameters_.turbulence.on == 1){
		
		// reset diffusive timestep stencil and determine min diffusive timestep
		minTimeStepStencil_.reset();
		minTimeStepIterator_.iterate();
		// store minimum diffusive timestep
		FLOAT diffusivetimeStep = minTimeStepStencil_.getDiffusiveTimeStep();
		/*
		if (parameters_.geometry.dim == 3) {
		    parameters_.timestep.dt = 1.0 / maxUStencil_.getMaxValues()[2];
		} else {
		    parameters_.timestep.dt = 1.0 / maxUStencil_.getMaxValues()[0];
		}
		*/
		//localMin = std::min(diffusivetimeStep, std::min(parameters_.timestep.dt, std::min(1 / maxUStencil_.getMaxValues()[0], 1 / maxUStencil_.getMaxValues()[1])));
		localMin = std::min(diffusivetimeStep, parameters_.timestep.dt);
		std::cout << "localMin: " << localMin << "   " << "diffusivetimeStep: " << diffusivetimeStep << "dt: " << parameters_.timestep.dt <<std::endl;
	}
	
	else{
		FLOAT factor = 1.0 / (parameters_.meshsize->getDxMin() * parameters_.meshsize->getDxMin()) + 1.0 / (parameters_.meshsize->getDyMin() * parameters_.meshsize->getDyMin());

		if (parameters_.geometry.dim == 3) {
		    factor += 1.0 / (parameters_.meshsize->getDzMin() * parameters_.meshsize->getDzMin());
		    parameters_.timestep.dt = 1.0 / maxUStencil_.getMaxValues()[2];
		} else {
		    parameters_.timestep.dt = 1.0 / maxUStencil_.getMaxValues()[0];
		}

		localMin = std::min(parameters_.flow.Re / (2 * factor), std::min(parameters_.timestep.dt, std::min(1 / maxUStencil_.getMaxValues()[0], 1 / maxUStencil_.getMaxValues()[1])));
		
    }
    
    // Here, we select the type of operation before compiling. This allows to use the correct
	// data type for MPI. Not a concern for small simulations, but useful if using heterogeneous
	// machines.

	globalMin = MY_FLOAT_MAX;
	MPI_Allreduce(&localMin, &globalMin, 1, MY_MPI_FLOAT, MPI_MIN, PETSC_COMM_WORLD);

	parameters_.timestep.dt = globalMin;
	parameters_.timestep.dt *= parameters_.timestep.tau;
	//parameters_.timestep.dt = 10e-5;
}


//*****Distance to nearest wall function

void Simulation::distanceNearestWall() {
	//number of cells in each direction plus the ghost cells 
    const int sizex = flowField_.getNx() + 3;
    const int sizey = flowField_.getNy() + 3;
    const int sizez = flowField_.getNz() + 3; 
    
    // distance to each axis
    FLOAT dx = 0; //dummy values
    FLOAT dy = 0; //dummy values
    FLOAT dz = 0; //dummy values;
    
    //in case there is a step geometry
    //number of cells in each direction that make up the step
    //parameters_.bfStep.xRatio and parameters_.bfStep.yRatio are -1 if no step
    int stepXBound = ceil(sizex*parameters_.bfStep.xRatio);
    int stepYBound = ceil(sizey*parameters_.bfStep.yRatio);
    
 	//field to store distance to wall
    ScalarField& distance_to_wall = flowField_.getDistance();


    if (parameters_.geometry.dim == 2) {
    	for (int j = 0; j < sizey; j++) {
    		for (int i = 0; i < sizex; i++) {
    			int obstacle = flowField_.getFlags().getValue(i, j);
    			//if cell is fluid:
    			if ((obstacle & OBSTACLE_SELF) == 0){

    				//check right wall is wall and check if cell is more than equal to half the domain width to the right
    				if ((obstacle = flowField_.getFlags().getValue(sizex-1, j) == 0) && (i >= sizex/2)){
    					dx = (sizex-i)*parameters_.meshsize->getDx(i,j) - parameters_.meshsize->getDx(i,j)/2;
    				}
    				//check left wall is wall and check if cell is less than half the domain width to the left
    				else if ((obstacle = flowField_.getFlags().getValue(0, j) == 0) && (i < sizex/2)){
    					dx = (i)*parameters_.meshsize->getDx(i,j) - parameters_.meshsize->getDx(i,j)/2;
    				}
    				//check top wall is wall and check if cell is more than equal to half the domain height to the top
    				if ((obstacle = flowField_.getFlags().getValue(i, sizey-1) == 0) && (j >= sizey/2)){
    					dy = (sizey-j)*parameters_.meshsize->getDy(i,j) - parameters_.meshsize->getDy(i,j)/2;
    				}
    				//check bottom wall is wall and check if cell is less than half the domain height to the bottom
    				else if ((obstacle = flowField_.getFlags().getValue(i, 0) == 0) && (j < sizey/2)){
    					dy = (j)*parameters_.meshsize->getDy(i,j) - parameters_.meshsize->getDy(i,j)/2;
    				}
    				
    				//find the distance of cell to nearest wall
    				distance_to_wall.getScalar(i,j) = std::abs(std::min(dx,dy));
				std::cout << "distance = " << distance_to_wall.getScalar(i,j) << "	dx = " 
					<< dx << "	dy = " << dy << std::endl;

    				
    				
    				//only goes into these loops if there is a step
    				//top boundary loop
    				for (int x=0; x < stepXBound; x++){
    					FLOAT xdis = (i-x)*parameters_.meshsize->getDx(i,j);
    					FLOAT ydis = (j-stepYBound)*parameters_.meshsize->getDy(i,j);
    					FLOAT stepDis = sqrt((xdis*xdis) + (ydis*ydis));

    					distance_to_wall.getScalar(i,j) = std::abs(std::min(distance_to_wall.getScalar(i,j), stepDis));   					
					std::cout << "distance = " << distance_to_wall.getScalar(i,j) << " stepDis = " 
					<< stepDis << std::endl;

    				}
    				//left boundary loop
    				for (int y=0; y < stepYBound; y++){
    					FLOAT xdis = (i-stepXBound)*parameters_.meshsize->getDx(i,j);
    					FLOAT ydis = (j-y)*parameters_.meshsize->getDy(i,j);
    					FLOAT stepDis = sqrt((xdis*xdis) + (ydis*ydis));

    					distance_to_wall.getScalar(i,j) = std::abs(std::min(distance_to_wall.getScalar(i,j), stepDis));   					
					std::cout << "left boundary distance = " << distance_to_wall.getScalar(i,j) << " stepDis = " 
					<< stepDis << std::endl;

    				}
    			}
    			//if cell is object then distance to wall is zero
    			else if ((obstacle & OBSTACLE_SELF) == 1){
    				distance_to_wall.getScalar(i,j) = 0;
    			}
    			
    			
    		}
    	}
           
    }
    
    if (parameters_.geometry.dim == 3) {
    	for (int k = 0; k < sizez; k++) {
			for (int j = 0; j < sizey; j++) {
				for (int i = 0; i < sizex; i++) {
					int obstacle = flowField_.getFlags().getValue(i, j, k);
					//if cell is fluid:
					if ((obstacle & OBSTACLE_SELF) == 0){
						//check right wall is wall and check if cell is more than equal to half the domain width to the right
						if ((obstacle = flowField_.getFlags().getValue(sizex-1, j, k) == 0) && (i >= sizex/2)){
							dx = (sizex-i)*parameters_.meshsize->getDx(i,j,k) - parameters_.meshsize->getDx(i,j,k)/2;
						}
						//check left wall is wall and check if cell is less than half the domain width to the left
						else if ((obstacle = flowField_.getFlags().getValue(0, j, k) == 0) && (i < sizex/2)){
							dx = (i)*parameters_.meshsize->getDx(i,j,k) - parameters_.meshsize->getDx(i,j,k)/2;
						}
						//check top wall is wall and check if cell is more than equal to half the domain height to the top
						if ((obstacle = flowField_.getFlags().getValue(i, sizey-1, k) == 0) && (j >= sizey/2)){
							dy = (sizey-j)*parameters_.meshsize->getDy(i,j,k) - parameters_.meshsize->getDy(i,j,k)/2;
						}
						//check bottom wall is wall and check if cell is less than half the domain height to the bottom
						else if ((obstacle = flowField_.getFlags().getValue(i, 0, k) == 0) && (j < sizey/2)){
							dy = (j)*parameters_.meshsize->getDy(i,j,k) - parameters_.meshsize->getDy(i,j,k)/2;
						}
						//check front wall is wall and check if cell is more than equal to half the domain depth to the start
						if ((obstacle = flowField_.getFlags().getValue(i, j, sizez-1) == 0) && (k >= sizez/2)){
							dz = (sizez-k)*parameters_.meshsize->getDx(i,j,k) - parameters_.meshsize->getDz(i,j,k)/2;
						}
						//check back wall is wall and check if cell is more than equal to half the domain depth to the end
						else if ((obstacle = flowField_.getFlags().getValue(i, j, 0) == 0) && (k < sizez/2)){
							dz = (k)*parameters_.meshsize->getDz(i,j,k) - parameters_.meshsize->getDz(i,j,k)/2;
						}
						
						//find the distance of cell to nearest wall
						distance_to_wall.getScalar(i,j,k) = std::abs(std::min(std::min(dx,dy),dz));
						
						//only goes into these loops if there is a step
						//we check both loops in the same z coordinate as the original point so dont need to count it
						//There is no step in the Z-axis
						//top boundary loop
						for (int x=0; x < stepXBound; x++){
							FLOAT xdis = (i-x)*parameters_.meshsize->getDx(i,j,k);
							FLOAT ydis = (j-stepYBound)*parameters_.meshsize->getDy(i,j,k);
							FLOAT stepDis = sqrt((xdis*xdis) + (ydis*ydis));
							distance_to_wall.getScalar(i,j,k) = std::abs(std::min(distance_to_wall.getScalar(i,j,k), stepDis));   					
						}
						//left boundary loop
						for (int y=0; y < stepYBound; y++){
							FLOAT xdis = (i-stepXBound)*parameters_.meshsize->getDx(i,j,k);
							FLOAT ydis = (j-y)*parameters_.meshsize->getDy(i,j,k);
							FLOAT stepDis = sqrt((xdis*xdis) + (ydis*ydis));
							distance_to_wall.getScalar(i,j,k) = std::abs(std::min(distance_to_wall.getScalar(i,j,k), stepDis));   					
						}
					}
					//if cell is object then distance to wall is zero
					else if ((obstacle & OBSTACLE_SELF) == 1){
						distance_to_wall.getScalar(i,j,k) = 0;
					}
					
					
				}
			}
		}       
    }
    
}
//**************************************

} // namespace NSEOF
