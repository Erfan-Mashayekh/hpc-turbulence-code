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

        pressureBufferLeft = pressureBufferFillStencil.getPressureBufferLeft();
        pressureBufferRight = pressureBufferFillStencil.getPressureBufferRight();

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

        pressureBufferTop = pressureBufferFillStencil.getPressureBufferTop();
        pressureBufferBottom = pressureBufferFillStencil.getPressureBufferBottom();

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

        pressureBufferFront = pressureBufferFillStencil.getPressureBufferFront();
        pressureBufferBack = pressureBufferFillStencil.getPressureBufferBack();

        MPI_Sendrecv(&pressureBufferBack[0], (int) pressureBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                     &pressureBufferFront[0], (int) pressureBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        pressureBufferReadStencil.setPressureBufferFrontIterator(pressureBufferFront);
    }

    void PetscParallelManager::communicateVelocity(Stencils::VelocityBufferFillStencil& velocityBufferFillStencil,
                                                   Stencils::VelocityBufferReadStencil& velocityBufferReadStencil) const {
        /**
         * @brief Communication: Send to right, receive from left
         */

        std::vector<FLOAT> velocityBufferLeft = velocityBufferFillStencil.getVelocityBufferLeft();
        std::vector<FLOAT> velocityBufferRight = velocityBufferFillStencil.getVelocityBufferRight();

        MPI_Sendrecv(&velocityBufferRight[0], (int) velocityBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                     &velocityBufferLeft[0], (int) velocityBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        velocityBufferReadStencil.setVelocityBufferLeftIterator(velocityBufferLeft);

        /**
         * @brief Communication: send to left, receive from right
         */

        velocityBufferLeft = velocityBufferFillStencil.getVelocityBufferLeft();
        velocityBufferRight = velocityBufferFillStencil.getVelocityBufferRight();

        MPI_Sendrecv(&velocityBufferLeft[0], (int) velocityBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                     &velocityBufferRight[0], (int) velocityBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        velocityBufferReadStencil.setVelocityBufferRightIterator(velocityBufferRight);

        /**
         * @brief Communication: send to top, receive from bottom
         */

        std::vector<FLOAT> velocityBufferTop = velocityBufferFillStencil.getVelocityBufferTop();
        std::vector<FLOAT> velocityBufferBottom = velocityBufferFillStencil.getVelocityBufferBottom();

        MPI_Sendrecv(&velocityBufferTop[0], (int) velocityBufferTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                     &velocityBufferBottom[0], (int) velocityBufferBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        velocityBufferReadStencil.setVelocityBufferBottomIterator(velocityBufferBottom);     

        /**
         * @brief Communication: send to bottom, receive from top
         */

        velocityBufferTop = velocityBufferFillStencil.getVelocityBufferTop();
        velocityBufferBottom = velocityBufferFillStencil.getVelocityBufferBottom();

        MPI_Sendrecv(&velocityBufferBottom[0], (int) velocityBufferBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                     &velocityBufferTop[0], (int) velocityBufferTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        velocityBufferReadStencil.setVelocityBufferTopIterator(velocityBufferTop);     

        /**
         * @brief Communication: send to front, receive from back
         */

        std::vector<FLOAT> velocityBufferFront = velocityBufferFillStencil.getVelocityBufferFront();
        std::vector<FLOAT> velocityBufferBack = velocityBufferFillStencil.getVelocityBufferBack();

        MPI_Sendrecv(&velocityBufferFront[0], (int) velocityBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                     &velocityBufferBack[0], (int) velocityBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        velocityBufferReadStencil.setVelocityBufferBackIterator(velocityBufferBack);     

        /**
         * @brief Communication: send to back, receive from front
         */

        velocityBufferFront = velocityBufferFillStencil.getVelocityBufferFront();
        velocityBufferBack = velocityBufferFillStencil.getVelocityBufferBack();

        MPI_Sendrecv(&velocityBufferBack[0], (int) velocityBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                     &velocityBufferFront[0], (int) velocityBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        velocityBufferReadStencil.setVelocityBufferFrontIterator(velocityBufferFront);
    }
} // namespace NSEOF::ParallelManagers
