#ifndef __TURBULENT_SIMULATION_HPP__
#define __TURBULENT_SIMULATION_HPP__

#include "Simulation.hpp"

#include "Stencils/MinTimeStepStencil.hpp"
#include "Stencils/DistanceStencil.hpp"
#include "Stencils/ViscosityStencil.hpp"

#include "Stencils/TurbulentFGHStencil.hpp"
#include "Stencils/TurbulentVTKStencil.hpp"

#include "ParallelManagers/TurbulentParallelManager.hpp"

namespace NSEOF {

class TurbulentSimulation : public Simulation {
private:
    Stencils::MinTimeStepStencil minTimeStepStencil_;
    FieldIterator<FlowField> minTimeStepIterator_;

    Stencils::DistanceStencil distanceStencil_;
    FieldIterator<FlowField> distanceIterator_;

    Stencils::ViscosityStencil viscosityStencil_;
    FieldIterator<FlowField> viscosityIterator_;

    ParallelManagers::TurbulentParallelManager turbulentParallelManager_;

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
