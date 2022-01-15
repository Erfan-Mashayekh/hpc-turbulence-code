#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include "FlowField.hpp"
#include "Iterators.hpp"
#include "Definitions.hpp"
#include "GlobalBoundaryFactory.hpp"

#include "Stencils/MovingWallStencils.hpp"
#include "Stencils/RHSStencil.hpp"
#include "Stencils/VelocityStencil.hpp"
#include "Stencils/ObstacleStencil.hpp"
#include "Stencils/VTKStencil.hpp"
#include "Stencils/MaxUStencil.hpp"
#include "Stencils/PeriodicBoundaryStencils.hpp"
#include "Stencils/BFStepInitStencil.hpp"
#include "Stencils/NeumannBoundaryStencils.hpp"
#include "Stencils/BFInputStencils.hpp"
#include "Stencils/InitTaylorGreenFlowFieldStencil.hpp"

#include "Stencils/FGHStencil.hpp"
#include "Stencils/VTKStencil.hpp"

#include "ParallelManagers/PetscParallelManager.hpp"

#include "Solvers/LinearSolver.hpp"

#include <memory>
#include <chrono>

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

    Stencils::RHSStencil rhsStencil_;
    FieldIterator<FlowField> rhsIterator_;

    Stencils::FGHStencil* fghStencil_;
    FieldIterator<FlowField>* fghIterator_;

    Stencils::VTKStencil* vtkStencil_;
    FieldIterator<FlowField>* vtkIterator_;

    Stencils::VelocityStencil velocityStencil_;
    Stencils::ObstacleStencil obstacleStencil_;
    FieldIterator<FlowField> velocityIterator_;
    FieldIterator<FlowField> obstacleIterator_;

    ParallelManagers::PetscParallelManager petscParallelManager_;

    std::unique_ptr<Solvers::LinearSolver> solver_;
    std::unique_ptr<Solvers::LinearSolver> solverEigen_;

    /** Gets the diffusive timestep and uses that to set the timestep before solving */
    virtual FLOAT getDiffusiveTimestep_();
    void setTimestep_();

public:
    Simulation(Parameters&, FlowField&);
    virtual ~Simulation();

    /** Initialises the flow field according to the scenario */
    virtual void initializeFlowField();

    /** Solves */
    virtual void solveTimestep(double&);

    /** Plots the flow field */
    void plotVTK(int);
};

} // namespace NSEOF

#endif // __SIMULATION_HPP__
