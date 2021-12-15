#include "TurbulentPetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    TurbulentPetscParallelManager::TurbulentPetscParallelManager(const Parameters& parameters)
        : PetscParallelManager(parameters) {}

    void TurbulentPetscParallelManager::communicateViscosity(Stencils::ViscosityBufferFillStencil& viscosityBufferFillStencil,
                                                             Stencils::ViscosityBufferReadStencil& viscosityBufferReadStencil) const {
      communicate_(viscosityBufferFillStencil, viscosityBufferReadStencil);
    }

} // namespace NSEOF::ParallelManagers
