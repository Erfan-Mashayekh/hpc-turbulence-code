#ifndef __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__
#define __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__

#include "PetscParallelManager.hpp"

#include "Stencils/ViscosityBufferFillStencil.hpp"
#include "Stencils/ViscosityBufferReadStencil.hpp"

namespace NSEOF::ParallelManagers {

class TurbulentPetscParallelManager : public PetscParallelManager {
public:
    explicit TurbulentPetscParallelManager(const Parameters&);
    ~TurbulentPetscParallelManager() override = default;

    void communicateViscosity(Stencils::ViscosityBufferFillStencil&, Stencils::ViscosityBufferReadStencil&) const;
};

} // namespace NSEOF::ParallelManagers

#endif // __PARALLEL_MANAGERS_TURBULENT_PETSC_PARALLEL_MANAGER_HPP__
