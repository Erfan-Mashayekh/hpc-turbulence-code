#ifndef __TURBULENT_PARALLEL_MANAGER_HPP__
#define __TURBULENT_PARALLEL_MANAGER_HPP__

#include "ParallelManager.hpp"

#include "Stencils/ViscosityBufferFillStencil.hpp"
#include "Stencils/ViscosityBufferReadStencil.hpp"

namespace NSEOF::ParallelManagers {

class TurbulentParallelManager : public ParallelManager {
private:
    Stencils::ViscosityBufferFillStencil viscosityBufferFillStencil_;
    Stencils::ViscosityBufferReadStencil viscosityBufferReadStencil_;

    ParallelBoundaryIterator<FlowField> viscosityBufferFillIterator_;
    ParallelBoundaryIterator<FlowField> viscosityBufferReadIterator_;

public:
    TurbulentParallelManager(const Parameters&, FlowField&);
    ~TurbulentParallelManager() override = default;

    void communicateViscosity();
};

} // namespace NSEOF::ParallelManagers

#endif // __TURBULENT_PARALLEL_MANAGER_HPP__
