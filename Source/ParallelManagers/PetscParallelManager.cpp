#include "PetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    PetscParallelManager::PetscParallelManager(const Parameters& parameters)
            : parameters_(parameters) {}

    void PetscParallelManager::communicatePressure(Stencils::PressureBufferFillStencil& pressureBufferFillStencil,
                                                   Stencils::PressureBufferReadStencil& pressureBufferReadStencil) const {
        std::vector<FLOAT> pressureBufferLeft = pressureBufferFillStencil.getPressureBufferLeft();
        std::vector<FLOAT> pressureBufferRight = pressureBufferFillStencil.getPressureBufferRight();

        // Communication!
        MPI_Sendrecv(&pressureBufferRight[0], (int) pressureBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                     &pressureBufferLeft[0], (int) pressureBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBufferLeftIterator(pressureBufferLeft);
    }

    void PetscParallelManager::communicateVelocity() const {

    }
} // namespace NSEOF::ParallelManagers
