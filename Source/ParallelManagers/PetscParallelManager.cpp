#include "PetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    void PetscParallelManager::communicatePressure(Stencils::PressureBufferFillStencil& pressureBufferFillStencil,
                                                   Stencils::PressureBufferReadStencil& pressureBufferReadStencil) {
        std::vector<FLOAT> pressureBufferLeft,   pressureBufferRight;
        std::vector<FLOAT> pressureBufferBottom, pressureBufferTop;
        std::vector<FLOAT> pressureBufferFront,  pressureBufferBack;

        pressureBufferFillStencil.getPressureBuffers(pressureBufferLeft,   pressureBufferRight,
                                                     pressureBufferBottom, pressureBufferTop,
                                                     pressureBufferFront,  pressureBufferBack);

        // Communication!
//        MPI_Sendrecv(&pressureBufferRight[0], SIZE, MY_MPI_FLOAT, left, 0,
//                     &pressureBufferLeft[0], SIZE, MY_MPI_FLOAT, right, 0,
//                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBuffers(pressureBufferLeft,   pressureBufferRight,
                                                     pressureBufferBottom, pressureBufferTop,
                                                     pressureBufferFront,  pressureBufferBack);
    }

    void PetscParallelManager::communicateVelocity() {

    }
} // namespace NSEOF::ParallelManagers
