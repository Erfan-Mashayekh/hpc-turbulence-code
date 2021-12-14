#ifndef __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__
#define __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__

#include "Parameters.hpp"
#include "Definitions.hpp"

#include "Stencils/PressureBufferFillStencil.hpp"
#include "Stencils/PressureBufferReadStencil.hpp"
#include "Stencils/VelocityBufferFillStencil.hpp"
#include "Stencils/VelocityBufferReadStencil.hpp"

namespace NSEOF::ParallelManagers {

class PetscParallelManager {
private:
    const Parameters& parameters_;

    void communicate_(Stencils::BufferFillStencil&, Stencils::BufferReadStencil&) const;

public:
    explicit PetscParallelManager(const Parameters&);
    virtual ~PetscParallelManager() = default;

    static void sendRecvBuffers(std::vector<FLOAT>&, int, std::vector<FLOAT>&, int);

    void communicatePressure(Stencils::PressureBufferFillStencil&, Stencils::PressureBufferReadStencil&) const;
    void communicateVelocity(Stencils::VelocityBufferFillStencil&, Stencils::VelocityBufferReadStencil&) const;
};

} // namespace NSEOF::ParallelManagers

#endif // __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__
