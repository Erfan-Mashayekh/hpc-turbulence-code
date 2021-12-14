#ifndef __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__
#define __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__

#include "PetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

class TurbulentPetscParallelManager : public PetscParallelManager {
public:
    explicit TurbulentPetscParallelManager(const Parameters&);
    ~TurbulentPetscParallelManager() override = default;

    // TODO: Implement the following communication files for Viscosity and uncomment!
    //  void communicateViscosity(Stencils::PressureBufferFillStencil&, Stencils::PressureBufferReadStencil&) const;
};

} // namespace NSEOF::ParallelManagers

#endif // __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__
