#ifndef __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__
#define __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__

#include "FlowField.hpp"
#include "Iterators.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include "Stencils/PressureBufferFillStencil.hpp"
#include "Stencils/PressureBufferReadStencil.hpp"
#include "Stencils/VelocityBufferFillStencil.hpp"
#include "Stencils/VelocityBufferReadStencil.hpp"

#include "Stencils/VelocityBufferDiagonalFillStencil.hpp"
#include "Stencils/VelocityBufferDiagonalReadStencil.hpp"

namespace NSEOF::ParallelManagers {

class PetscParallelManager {
private:
    const Parameters& parameters_;
    FlowField& flowField_;

    NSEOF::Stencils::PressureBufferFillStencil pressureBufferFillStencil_;
    NSEOF::Stencils::PressureBufferReadStencil pressureBufferReadStencil_;
    NSEOF::Stencils::VelocityBufferFillStencil velocityBufferFillStencil_;
    NSEOF::Stencils::VelocityBufferReadStencil velocityBufferReadStencil_;

    NSEOF::Stencils::VelocityBufferDiagonalFillStencil velocityBufferDiagonalFillStencil_;
    NSEOF::Stencils::VelocityBufferDiagonalReadStencil velocityBufferDiagonalReadStencil_;

    ParallelBoundaryIterator<FlowField> pressureBufferFillIterator_;
    ParallelBoundaryIterator<FlowField> pressureBufferReadIterator_;
    ParallelBoundaryIterator<FlowField> velocityBufferFillIterator_;
    ParallelBoundaryIterator<FlowField> velocityBufferReadIterator_;

    ParallelBoundaryDiagonalIterator<FlowField> velocityBufferFillDiagonalIterator_;
    ParallelBoundaryDiagonalIterator<FlowField> velocityBufferReadDiagonalIterator_;

protected:
    void communicate_(Stencils::BufferFillStencil&, Stencils::BufferReadStencil&) const;
    void communicateDiagonals_(Stencils::BufferFillStencil&, Stencils::BufferReadStencil&) const;

public:
    PetscParallelManager(const Parameters&, FlowField&);
    virtual ~PetscParallelManager() = default;

    static void sendRecvBuffers(std::vector<FLOAT>&, int, std::vector<FLOAT>&, int);

    void communicatePressure();
    void communicateVelocity();
};

} // namespace NSEOF::ParallelManagers

#endif // __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__
