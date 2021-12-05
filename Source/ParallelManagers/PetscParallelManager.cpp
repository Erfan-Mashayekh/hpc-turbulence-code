#include "PetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    PetscParallelManager::PetscParallelManager(const Parameters& parameters)
            : parameters_(parameters) {}

    void PetscParallelManager::communicatePressure(Stencils::PressureBufferFillStencil& pressureBufferFillStencil,
                                                   Stencils::PressureBufferReadStencil& pressureBufferReadStencil) const {

        /**
         * @brief Communication: Send to right, receive from left
         */

        std::vector<FLOAT> pressureBufferLeft = pressureBufferFillStencil.getPressureBufferLeft();
        std::vector<FLOAT> pressureBufferRight = pressureBufferFillStencil.getPressureBufferRight();

        MPI_Sendrecv(&pressureBufferRight[0], (int) pressureBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                     &pressureBufferLeft[0], (int) pressureBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBufferLeftIterator(pressureBufferLeft);

        /**
         * @brief Communication: send to left, receive from right
         */

        std::vector<FLOAT> pressureBufferLeft = pressureBufferFillStencil.getPressureBufferLeft();
        std::vector<FLOAT> pressureBufferRight = pressureBufferFillStencil.getPressureBufferRight();

        MPI_Sendrecv(&pressureBufferLeft[0], (int) pressureBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                     &pressureBufferRight[0], (int) pressureBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBufferRightIterator(pressureBufferRight);

        /**
         * @brief Communication: send to top, receive from bottom
         */

        std::vector<FLOAT> pressureBufferTop = pressureBufferFillStencil.getPressureBufferTop();
        std::vector<FLOAT> pressureBufferBottom = pressureBufferFillStencil.getPressureBufferBottom();

        MPI_Sendrecv(&pressureBufferTop[0], (int) pressureBufferTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                     &pressureBufferBottom[0], (int) pressureBufferBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBufferBottomIterator(pressureBufferBottom);     

        /**
         * @brief Communication: send to bottom, receive from top
         */

        std::vector<FLOAT> pressureBufferTop = pressureBufferFillStencil.getPressureBufferTop();
        std::vector<FLOAT> pressureBufferBottom = pressureBufferFillStencil.getPressureBufferBottom();

        MPI_Sendrecv(&pressureBufferBottom[0], (int) pressureBufferBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                     &pressureBufferTop[0], (int) pressureBufferTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBufferTopIterator(pressureBufferTop);     

        /**
         * @brief Communication: send to front, receive from back
         */

        std::vector<FLOAT> pressureBufferFront = pressureBufferFillStencil.getPressureBufferFront();
        std::vector<FLOAT> pressureBufferBack = pressureBufferFillStencil.getPressureBufferBack();

        MPI_Sendrecv(&pressureBufferFront[0], (int) pressureBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                     &pressureBufferBack[0], (int) pressureBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBufferBackIterator(pressureBufferBack);     

        /**
         * @brief Communication: send to back, receive from front
         */

        std::vector<FLOAT> pressureBufferFront = pressureBufferFillStencil.getPressureBufferFront();
        std::vector<FLOAT> pressureBufferBack = pressureBufferFillStencil.getPressureBufferBack();

        MPI_Sendrecv(&pressureBufferBack[0], (int) pressureBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                     &pressureBufferFront[0], (int) pressureBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBufferFrontIterator(pressureBufferFront);     

    }

    void PetscParallelManager::communicateVelocity(Stencils::VelocityBufferFillStencil& velocityBufferFillStencil,
                                                   Stencils::VelocityBufferReadStencil& velocityBufferReadStencil) const {
    }
} // namespace NSEOF::ParallelManagers
