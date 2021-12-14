#ifndef __TURBULENT_SIMULATION_HPP__
#define __TURBULENT_SIMULATION_HPP__

#include "Simulation.hpp"

#include "Stencils/TurbulentFGHStencil.hpp"
#include "Stencils/TurbulentVTKStencil.hpp"

#include "Stencils/ViscosityStencil.hpp"
#include "Stencils/MinTimeStepStencil.hpp"

#include "ParallelManagers/TurbulentPetscParallelManager.hpp"

// TODO: Implement the following communication files for Viscosity and uncomment!
//  #include "Stencils/ViscosityBufferFillStencil.hpp"
//  #include "Stencils/ViscosityBufferReadStencil.hpp"

#include <limits>

namespace NSEOF {

class TurbulentSimulation : public Simulation {
private:
    Stencils::TurbulentFGHStencil turbulentFghStencil_;
    FieldIterator<FlowField> turbulentFghIterator_;

    Stencils::ViscosityStencil viscosityStencil_;
    FieldIterator<FlowField> viscosityIterator_;

    Stencils::MinTimeStepStencil minTimeStepStencil_;
    FieldIterator<FlowField> minTimeStepIterator_;

    ParallelManagers::TurbulentPetscParallelManager turbulentPetscParallelManager_;

    // TODO: Implement the following communication files for Viscosity and uncomment!
    //  Stencils::ViscosityBufferFillStencil viscosityBufferFillStencil_;
    //  Stencils::ViscosityBufferReadStencil viscosityBufferReadStencil_;

protected:
    /** Gets the diffusive timestep for setting the timestep before solving */
    FLOAT getDiffusiveTimestep_() override;

    /** Iterates and computes FGH values and communicates if needed */
    void iterateFGHValues_() override;

public:
    TurbulentSimulation(Parameters& parameters, FlowField& flowField);
    ~TurbulentSimulation() override = default;

    /** Calculates the distances to the nearest walls */
    void calculateDistancesToNearestWalls();

    /** Initialises the flow field according to the scenario */
    void initializeFlowField() override;

    /** Plots the flow field */
    void plotVTK(int timeStep) override;
};

} // namespace NSEOF

#endif // __TURBULENT_SIMULATION_HPP__
