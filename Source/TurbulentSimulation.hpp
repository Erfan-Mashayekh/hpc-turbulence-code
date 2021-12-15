#ifndef __TURBULENT_SIMULATION_HPP__
#define __TURBULENT_SIMULATION_HPP__

#include "Simulation.hpp"

#include "Stencils/MinTimeStepStencil.hpp"
#include "Stencils/DistanceStencil.hpp"
#include "Stencils/ViscosityStencil.hpp"

#include "Stencils/TurbulentFGHStencil.hpp"
#include "Stencils/TurbulentVTKStencil.hpp"

#include "ParallelManagers/TurbulentPetscParallelManager.hpp"

#include "Stencils/ViscosityBufferFillStencil.hpp"
#include "Stencils/ViscosityBufferReadStencil.hpp"

namespace NSEOF {

class TurbulentSimulation : public Simulation {
private:
    Stencils::MinTimeStepStencil minTimeStepStencil_;
    FieldIterator<FlowField> minTimeStepIterator_;

    Stencils::DistanceStencil distanceStencil_;
    FieldIterator<FlowField> distanceIterator_;

    Stencils::ViscosityStencil viscosityStencil_;
    FieldIterator<FlowField> viscosityIterator_;

    ParallelManagers::TurbulentPetscParallelManager turbulentPetscParallelManager_;

    Stencils::ViscosityBufferFillStencil viscosityBufferFillStencil_;
    Stencils::ViscosityBufferReadStencil viscosityBufferReadStencil_;

    ParallelBoundaryIterator<FlowField> viscosityBufferFillIterator_;
    ParallelBoundaryIterator<FlowField> viscosityBufferReadIterator_;

protected:
    /** Gets the diffusive timestep for setting the timestep before solving */
    FLOAT getDiffusiveTimestep_() override;

public:
    TurbulentSimulation(Parameters&, FlowField&);
    ~TurbulentSimulation() override = default;

    /** Initialises the flow field according to the scenario */
    void initializeFlowField() override;

    /** Solves */
    void solveTimestep() override;
};

} // namespace NSEOF

#endif // __TURBULENT_SIMULATION_HPP__
