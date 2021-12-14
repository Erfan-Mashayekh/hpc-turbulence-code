#include "TurbulentSimulation.hpp"

namespace NSEOF {

TurbulentSimulation::TurbulentSimulation(Parameters& parameters, FlowField& flowField)
    : Simulation(parameters, flowField)
    , viscosityStencil_(parameters)
    , viscosityIterator_(flowField, parameters, viscosityStencil_)
    , minTimeStepStencil_(parameters)
    , minTimeStepIterator_(flowField, parameters, minTimeStepStencil_)
    , turbulentPetscParallelManager_(parameters)
    // TODO: Implement the following communication files for Viscosity and uncomment!
    //  , viscosityBufferFillStencil_(parameters)
    //  , viscosityBufferReadStencil_(parameters)
    //  , viscosityBufferFillIterator_(flowField, parameters, viscosityBufferFillStencil_,
    //                                 parameters_.vtk.whiteRegionLowOffset, parameters_.vtk.whiteRegionHighOffset)
    //  , viscosityBufferReadIterator_(flowField, parameters, viscosityBufferReadStencil_,
    //
{
    fghStencil_  = new Stencils::TurbulentFGHStencil(parameters_);
    fghIterator_ = new FieldIterator<FlowField>(flowField_, parameters_, *fghStencil_);

    vtkStencil_  = new Stencils::TurbulentVTKStencil(parameters_,
                                                     flowField_.getCellsX(), flowField_.getCellsY(), flowField_.getCellsZ());
    vtkIterator_ = new FieldIterator<FlowField>(flowField_, parameters_, *vtkStencil_);
}

// TODO: Needs refactoring! Maybe create a SimulationMechanics class..
void TurbulentSimulation::calculateDistancesToNearestWalls() {
    // Number of cells in each direction plus the ghost cells
    const int sizeX = flowField_.getNx() + 3;
    const int sizeY = flowField_.getNy() + 3;
    const int sizeZ = parameters_.geometry.dim == 3 ? flowField_.getNz() + 3 : 1;

    // Distance to each axis
    FLOAT distX, distY, distZ = std::numeric_limits<FLOAT>::infinity();

    /**
     * In case there is a step geometry,
     * number of cells in each direction that make up the step
     * parameters.bfStep.xRatio and parameters.bfStep.yRatio are -1 if no step
     */
    int stepXBound = ceil(sizeX * parameters_.bfStep.xRatio);
    int stepYBound = ceil(sizeY * parameters_.bfStep.yRatio);

    // Field to store distance to wall
    ScalarField& distance_to_wall = flowField_.getDistance();

    for (int k = 0; k < sizeZ; k++) {
        for (int j = 0; j < sizeY; j++) {
            for (int i = 0; i < sizeX; i++) {
                int obstacle = flowField_.getFlags().getValue(i, j, k);

                if ((obstacle & OBSTACLE_SELF) != 0) { // If it is not a fluid cell, the dist is zero.
                    distance_to_wall.getScalar(i, j, k) = 0;
                } else { // If it is a fluid cell, calculate the distance
                	/**
                	* Note: To limit if conditions we are only checking for u-velocities on each wall
                	*/

                	// check if left or right wall has u-velocity
                	if ((parameters_.walls.vectorLeft[0] == 0) && (parameters_.walls.vectorRight[0] == 0)) {
                    	distX = std::abs((i <= sizeX / 2 ? i : sizeX - i) * parameters_.meshsize->getDx(i, j, k));
                    } else if ((parameters_.walls.vectorLeft[0] == 0) && (parameters_.walls.vectorRight[0] != 0)) {
                    	distX = std::abs(i * parameters_.meshsize->getDx(i, j, k));
                    } else if ((parameters_.walls.vectorLeft[0] != 0) && (parameters_.walls.vectorRight[0] == 0)) {
                    	distX = std::abs((sizeX - i) * parameters_.meshsize->getDx(i, j, k));
                    } else {
                    	distX = std::numeric_limits<FLOAT>::infinity();
                    }

                    // check if top or bottom wall has u-velocity
                	if ((parameters_.walls.vectorBottom[0] == 0) && (parameters_.walls.vectorTop[0] == 0)) {
                    	distY = std::abs((j <= sizeY / 2 ? j : sizeY - j) * parameters_.meshsize->getDy(i, j, k));
                    } else if ((parameters_.walls.vectorBottom[0] == 0) && (parameters_.walls.vectorTop[0] != 0)) {
                    	distY = std::abs(j * parameters_.meshsize->getDy(i, j, k));
                    } else if ((parameters_.walls.vectorBottom[0] != 0) && (parameters_.walls.vectorTop[0] == 0)) {
                    	distY = std::abs((sizeY - j) * parameters_.meshsize->getDy(i, j, k));
                    } else {
                    	distY = std::numeric_limits<FLOAT>::infinity();
                    }

                    if (parameters_.geometry.dim == 3) { // 3D
                        // check if front or back wall has u-velocity
		            	if ((parameters_.walls.vectorBack[0] == 0) && (parameters_.walls.vectorFront[0] == 0)) {
		                	distZ = std::abs((k <= sizeZ / 2 ? k : sizeZ - k) * parameters_.meshsize->getDz(i, j, k));
		                } else if ((parameters_.walls.vectorBack[0] == 0) && (parameters_.walls.vectorFront[0] != 0)) {
		                	distZ = std::abs(k * parameters_.meshsize->getDz(i, j, k));
		                } else if ((parameters_.walls.vectorBottom[0] != 0) && (parameters_.walls.vectorTop[0] == 0)) {
		                	distZ = std::abs((sizeZ - k) * parameters_.meshsize->getDz(i, j, k));
		                } else {
		                	distZ = std::numeric_limits<FLOAT>::infinity();
		                }
                    }

                    // Find the distance of cell to the nearest wall
                    distance_to_wall.getScalar(i, j, k) = std::abs(std::min(std::min(distX, distY), distZ));

                    /**
                     * Only goes into the following loops if there is a step
                     *
                     * Note: we check both loops in the same z coordinate as the original point
                     *       so don't need to count it as there is no step in the Z-axis
                     */

                    // top boundary loop
                    for (int x = 0; x < stepXBound; x++) {
                        FLOAT dx = (i - x) * parameters_.meshsize->getDx(i, j, k);
                        FLOAT dy = (j - stepYBound) * parameters_.meshsize->getDy(i, j, k);
                        FLOAT stepDis = sqrt((dx * dx) + (dy * dy));

                        distance_to_wall.getScalar(i, j, k) = std::abs(std::min(distance_to_wall.getScalar(i, j, k), stepDis));
                    }

                    // left boundary loop
                    for (int y = 0; y < stepYBound; y++) {
                        FLOAT dx = (i - stepXBound) * parameters_.meshsize->getDx(i, j, k);
                        FLOAT dy = (j - y) * parameters_.meshsize->getDy(i, j, k);
                        FLOAT stepDis = sqrt((dx * dx) + (dy * dy));

                        distance_to_wall.getScalar(i, j, k) = std::abs(std::min(distance_to_wall.getScalar(i, j, k), stepDis));
                    }
                }

                // TODO: Remove the following print!
                std::cout << "distance to wall " << distance_to_wall.getScalar(i, j, k) << "	dx " << distX <<"	dy " << distY << "	dz " << distZ << "	i " << i << "	j " << j << "	k " << k << std::endl;
            }
        }
    }
}

void TurbulentSimulation::initializeFlowField() {
    Simulation::initializeFlowField();
    TurbulentSimulation::calculateDistancesToNearestWalls();
}

FLOAT TurbulentSimulation::getDiffusiveTimestep_() {
    // reset diffusive timestep stencil and determine min diffusive timestep
    minTimeStepStencil_.reset();
    minTimeStepIterator_.iterate();

    // return the minimum diffusive timestep
    return minTimeStepStencil_.getDiffusiveTimeStep();
}

void TurbulentSimulation::solveTimestep() {
    Simulation::solveTimestep();

    viscosityIterator_.iterate(); // Compute eddy viscosity

    // TODO WS2: communicate viscosity values
    //  viscosityBufferFillIterator_.iterate();
    //  turbulentPetscParallelManager_.communicateViscosity(viscosityBufferFillStencil_, viscosityBufferReadStencil_);
    //  viscosityBufferReadIterator_.iterate();
}

} // namespace NSEOF
