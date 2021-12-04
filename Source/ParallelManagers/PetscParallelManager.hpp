#ifndef __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__
#define __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__


namespace NSEOF::ParallelManagers {

class PetscParallelManager {

public:
    PetscParallelManager() = default;
    ~PetscParallelManager() = default;

    void communicatePressure();
    void communicateVelocity();
};

} // namespace NSEOF::ParallelManagers

#endif // __PARALLEL_MANAGERS_PETSC_PARALLEL_MANAGER_HPP__
