#include "TurbulentParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    TurbulentParallelManager::TurbulentParallelManager(const Parameters& parameters, FlowField& flowField)
            : ParallelManager(parameters, flowField)
            , viscosityBufferFillStencil_(parameters)
            , viscosityBufferReadStencil_(parameters)
            , viscosityBufferFillIterator_(flowField, parameters, viscosityBufferFillStencil_,
                                          parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
            , viscosityBufferReadIterator_(flowField, parameters, viscosityBufferReadStencil_,
                                          parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset) {}

    void TurbulentParallelManager::communicateViscosity() {
        viscosityBufferFillIterator_.iterate();
        communicate_(viscosityBufferFillStencil_, viscosityBufferReadStencil_);
        viscosityBufferReadIterator_.iterate();
    }

} // namespace NSEOF::ParallelManagers
