#ifndef __TURBULENT_SIMULATION_HPP__
#define __TURBULENT_SIMULATION_HPP__

#include "Simulation.hpp"

#include "Stencils/MinTimeStepStencil.hpp"
#include "Stencils/ViscosityStencil.hpp"
#include "Stencils/DistanceStencil.hpp"

#include "Stencils/TurbulentFGHStencil.hpp"
#include "Stencils/TurbulentVTKStencil.hpp"

#include "ParallelManagers/TurbulentPetscParallelManager.hpp"

// TODO: Implement the following communication files for Viscosity and uncomment!
//  #include "Stencils/ViscosityBufferFillStencil.hpp"
//  #include "Stencils/ViscosityBufferReadStencil.hpp"

namespace NSEOF {

class TurbulentSimulation : public Simulation {
private:
    Stencils::MinTimeStepStencil minTimeStepStencil_;
    FieldIterator<FlowField> minTimeStepIterator_;

    Stencils::ViscosityStencil viscosityStencil_;
    FieldIterator<FlowField> viscosityIterator_;

    Stencils::DistanceStencil distanceStencil_;
    FieldIterator<FlowField> distanceIterator_;

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

    /** Solves */
    void solveTimestep() override;
};

} // namespace NSEOF

#endif // __TURBULENT_SIMULATION_HPP__
