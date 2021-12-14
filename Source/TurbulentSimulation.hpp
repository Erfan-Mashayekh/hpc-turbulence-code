#ifndef __TURBULENT_SIMULATION_HPP__
#define __TURBULENT_SIMULATION_HPP__

#include "Simulation.hpp"

#include "Stencils/ViscosityStencil.hpp"
#include "Stencils/MinTimeStepStencil.hpp"

#include "Stencils/TurbulentFGHStencil.hpp"
#include "Stencils/TurbulentVTKStencil.hpp"

#include "ParallelManagers/TurbulentPetscParallelManager.hpp"

// TODO: Implement the following communication files for Viscosity and uncomment!
//  #include "Stencils/ViscosityBufferFillStencil.hpp"
//  #include "Stencils/ViscosityBufferReadStencil.hpp"

#include <limits>

namespace NSEOF {

class TurbulentSimulation : public Simulation {
private:
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

public:
    TurbulentSimulation(Parameters&, FlowField&);
    ~TurbulentSimulation() override = default;

    /** Calculates the distances to the nearest walls */
    void calculateDistancesToNearestWalls();

    /** Initialises the flow field according to the scenario */
    void initializeFlowField() override;

    /** Solves */
    void solveTimestep() override;
};

} // namespace NSEOF

#endif // __TURBULENT_SIMULATION_HPP__
