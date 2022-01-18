#ifndef __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__
#define __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__

#include "PetscParallelManager.hpp"

#include "Stencils/ViscosityBufferFillStencil.hpp"
#include "Stencils/ViscosityBufferReadStencil.hpp"

namespace NSEOF::ParallelManagers {

class TurbulentPetscParallelManager : public PetscParallelManager {
private:
    Stencils::ViscosityBufferFillStencil viscosityBufferFillStencil_;
    Stencils::ViscosityBufferReadStencil viscosityBufferReadStencil_;

    ParallelBoundaryIterator<FlowField> viscosityBufferFillIterator_;
    ParallelBoundaryIterator<FlowField> viscosityBufferReadIterator_;

public:
    TurbulentPetscParallelManager(const Parameters&, FlowField&);
    ~TurbulentPetscParallelManager() override = default;

    void communicateViscosity();
};

} // namespace NSEOF::ParallelManagers

#endif // __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__
