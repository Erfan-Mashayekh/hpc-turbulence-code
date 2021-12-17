#include "TurbulentPetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    TurbulentPetscParallelManager::TurbulentPetscParallelManager(const Parameters& parameters, FlowField& flowField)
            : PetscParallelManager(parameters, flowField)
            , viscosityBufferFillStencil_(parameters)
            , viscosityBufferReadStencil_(parameters)
            , viscosityBufferFillIterator_(flowField, parameters, viscosityBufferFillStencil_,
                                          parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
            , viscosityBufferReadIterator_(flowField, parameters, viscosityBufferReadStencil_,
                                          parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset) {}

    void TurbulentPetscParallelManager::communicateViscosity() {
        viscosityBufferFillIterator_.iterate();
        communicate_(viscosityBufferFillStencil_, viscosityBufferReadStencil_);
        viscosityBufferReadIterator_.iterate();
    }

} // namespace NSEOF::ParallelManagers
