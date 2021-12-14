#ifndef __TURBULENT_SIMULATION_HPP__
#define __TURBULENT_SIMULATION_HPP__

#include "Simulation.hpp"

#include "Stencils/TurbulentFGHStencil.hpp"
#include "Stencils/TurbulentVTKStencil.hpp"

#include "Stencils/ViscosityStencil.hpp"
#include "Stencils/MinTimeStepStencil.hpp"

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

protected:
    /** Gets the diffusive timestep for setting the timestep before solving */
    FLOAT getDiffusiveTimestep_() override;

    /** Iterates and computes FGH values and communicates if needed */
    void iterateFGHValues_() override;

public:
    TurbulentSimulation(Parameters& parameters, FlowField& flowField);
    virtual ~TurbulentSimulation() = default;

    /** Calculates the distances to the nearest walls */
    void calculateDistancesToNearestWalls();

    /** Initialises the flow field according to the scenario */
    void initializeFlowField() override;

    /** Plots the flow field */
    void plotVTK(int timeStep) override;
};

} // namespace NSEOF

#endif // __TURBULENT_SIMULATION_HPP__
