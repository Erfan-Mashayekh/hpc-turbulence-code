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

public:
    PetscParallelManager() = default;
    ~PetscParallelManager() = default;

    static void communicatePressure(Stencils::PressureBufferFillStencil&, Stencils::PressureBufferReadStencil&);
    static void communicateVelocity();
};

} // namespace NSEOF::ParallelManagers

#endif // __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__
