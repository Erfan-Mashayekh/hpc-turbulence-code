#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include "FlowField.hpp"
#include "Iterators.hpp"
#include "Definitions.hpp"
#include "GlobalBoundaryFactory.hpp"

#include "Stencils/FGHStencil.hpp"
#include "Stencils/MovingWallStencils.hpp"
#include "Stencils/RHSStencil.hpp"
#include "Stencils/VelocityStencil.hpp"
#include "Stencils/ViscosityStencil.hpp"
#include "Stencils/MinTimeStepStencil.hpp"
#include "Stencils/ObstacleStencil.hpp"
#include "Stencils/VTKStencil.hpp"
#include "Stencils/MaxUStencil.hpp"
#include "Stencils/PeriodicBoundaryStencils.hpp"
#include "Stencils/BFStepInitStencil.hpp"
#include "Stencils/NeumannBoundaryStencils.hpp"
#include "Stencils/BFInputStencils.hpp"
#include "Stencils/InitTaylorGreenFlowFieldStencil.hpp"

#include "Solvers/LinearSolver.hpp"


#include <memory>

namespace NSEOF {

class Simulation {
protected:
    Parameters& parameters_;

    FlowField& flowField_;

    Stencils::MaxUStencil maxUStencil_;
    FieldIterator<FlowField> maxUFieldIterator_;
    GlobalBoundaryIterator<FlowField> maxUBoundaryIterator_;

    // Set up the boundary conditions
    GlobalBoundaryFactory globalBoundaryFactory_;
    GlobalBoundaryIterator<FlowField> wallVelocityIterator_;
    GlobalBoundaryIterator<FlowField> wallFGHIterator_;

    Stencils::FGHStencil fghStencil_;
    FieldIterator<FlowField> fghIterator_;

    Stencils::RHSStencil rhsStencil_;
    FieldIterator<FlowField> rhsIterator_;

    Stencils::VelocityStencil velocityStencil_;
    Stencils::ObstacleStencil obstacleStencil_;
    FieldIterator<FlowField> velocityIterator_;
    FieldIterator<FlowField> obstacleIterator_;

    Stencils::ViscosityStencil viscosityStencil_;
    FieldIterator<FlowField> viscosityIterator_;
    
    Stencils::MinTimeStepStencil minTimeStepStencil_;
    FieldIterator<FlowField> minTimeStepIterator_;
    
    std::unique_ptr<Solvers::LinearSolver> solver_;

    virtual void setTimeStep();

public:
    Simulation(Parameters& parameters, FlowField& flowField);
    virtual ~Simulation() = default;

    /** Initialises the flow field according to the scenario */
    virtual void initializeFlowField();

    virtual void solveTimestep();

    /** Plots the flow field */
    virtual void plotVTK(int timeStep);
    
    /** calculates distance to nearest wall */
    virtual void distanceNearestWall();
};

/*
//TODO: is this right?!
class TurbulentSimulation:public Simulation{
	public:
		TurbulentSimulation(Parameters& parameters, FlowField& flowField):Simulation(parameters, flowField);
};
*/

} // namespace NSEOF

#endif // __SIMULATION_HPP__
