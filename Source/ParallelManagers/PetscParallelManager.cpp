#include "PetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    PetscParallelManager::PetscParallelManager(const Parameters& parameters)
            : parameters_(parameters) {}

    void PetscParallelManager::communicatePressure(Stencils::PressureBufferFillStencil& pressureBufferFillStencil,
                                                   Stencils::PressureBufferReadStencil& pressureBufferReadStencil) const {
        /**
         * Communication: Left & Right
         */
        if (parameters_.parallel.rightNb != MPI_PROC_NULL || parameters_.parallel.leftNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to left, receive from right
             */

            std::vector<FLOAT> pressureBufferLeft = pressureBufferFillStencil.getPressureBufferLeft();
            std::vector<FLOAT> pressureBufferRight = pressureBufferFillStencil.getPressureBufferRight();

            MPI_Sendrecv(&pressureBufferLeft[0], (int) pressureBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                         &pressureBufferRight[0], (int) pressureBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.rightNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferRightIterator(pressureBufferRight);
            }

            /**
             * Communication: Send to right, receive from left
             */

            pressureBufferLeft = pressureBufferFillStencil.getPressureBufferLeft();
            pressureBufferRight = pressureBufferFillStencil.getPressureBufferRight();

            MPI_Sendrecv(&pressureBufferRight[0], (int) pressureBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                         &pressureBufferLeft[0], (int) pressureBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.leftNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferLeftIterator(pressureBufferLeft);
            }
        }
        return;
        /**
         * Communication: Bottom & Top
         */
        if (parameters_.parallel.bottomNb != MPI_PROC_NULL || parameters_.parallel.topNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to bottom, receive from top
             */

            std::vector<FLOAT> pressureBufferTop = pressureBufferFillStencil.getPressureBufferTop();
            std::vector<FLOAT> pressureBufferBottom = pressureBufferFillStencil.getPressureBufferBottom();

            MPI_Sendrecv(&pressureBufferBottom[0], (int) pressureBufferBottom.size(), MY_MPI_FLOAT,
                         parameters_.parallel.bottomNb, 0,
                         &pressureBufferTop[0], (int) pressureBufferTop.size(), MY_MPI_FLOAT,
                         parameters_.parallel.topNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.topNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferTopIterator(pressureBufferTop);
            }

            /**
             * Communication: Send to top, receive from bottom
             */

            pressureBufferTop = pressureBufferFillStencil.getPressureBufferTop();
            pressureBufferBottom = pressureBufferFillStencil.getPressureBufferBottom();

            MPI_Sendrecv(&pressureBufferTop[0], (int) pressureBufferTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                         &pressureBufferBottom[0], (int) pressureBufferBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.bottomNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferBottomIterator(pressureBufferBottom);
            }
        }

        /**
         * Communication: Front & Back
         */
        if (parameters_.parallel.frontNb != MPI_PROC_NULL || parameters_.parallel.backNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to front, receive from back
             */

            std::vector<FLOAT> pressureBufferFront = pressureBufferFillStencil.getPressureBufferFront();
            std::vector<FLOAT> pressureBufferBack = pressureBufferFillStencil.getPressureBufferBack();

            MPI_Sendrecv(&pressureBufferFront[0], (int) pressureBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                         &pressureBufferBack[0], (int) pressureBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.backNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferBackIterator(pressureBufferBack);
            }

            /**
             * Communication: Send to back, receive from front
             */

            pressureBufferFront = pressureBufferFillStencil.getPressureBufferFront();
            pressureBufferBack = pressureBufferFillStencil.getPressureBufferBack();

            MPI_Sendrecv(&pressureBufferBack[0], (int) pressureBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                         &pressureBufferFront[0], (int) pressureBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.frontNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferFrontIterator(pressureBufferFront);
            }
        }

        pressureBufferFillStencil.clearPressureBuffers();
    }

    void PetscParallelManager::communicateVelocity(Stencils::VelocityBufferFillStencil& velocityBufferFillStencil,
                                                   Stencils::VelocityBufferReadStencil& velocityBufferReadStencil) const {
        return;
        /**
         * Communication: Left & Right
         */
        if (parameters_.parallel.rightNb != MPI_PROC_NULL || parameters_.parallel.leftNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to left, receive from right
             */

            std::vector<FLOAT> velocityBufferLeft = velocityBufferFillStencil.getVelocityBufferLeft();
            std::vector<FLOAT> velocityBufferRight = velocityBufferFillStencil.getVelocityBufferRight();

            MPI_Sendrecv(&velocityBufferLeft[0], (int) velocityBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                         &velocityBufferRight[0], (int) velocityBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.rightNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferRightIterator(velocityBufferRight);
            }

            /**
             * Communication: Send to right, receive from left
             */

            velocityBufferLeft = velocityBufferFillStencil.getVelocityBufferLeft();
            velocityBufferRight = velocityBufferFillStencil.getVelocityBufferRight();

            MPI_Sendrecv(&velocityBufferRight[0], (int) velocityBufferRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                         &velocityBufferLeft[0], (int) velocityBufferLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.leftNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferLeftIterator(velocityBufferLeft);
            }
        }

        /**
         * Communication: Bottom & Top
         */
        if (parameters_.parallel.bottomNb != MPI_PROC_NULL || parameters_.parallel.topNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to bottom, receive from top
             */

            std::vector<FLOAT> velocityBufferTop = velocityBufferFillStencil.getVelocityBufferTop();
            std::vector<FLOAT> velocityBufferBottom = velocityBufferFillStencil.getVelocityBufferBottom();

            MPI_Sendrecv(&velocityBufferBottom[0], (int) velocityBufferBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                         &velocityBufferTop[0], (int) velocityBufferTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.topNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferTopIterator(velocityBufferTop);
            }

            /**
             * Communication: Send to top, receive from bottom
             */

            velocityBufferTop = velocityBufferFillStencil.getVelocityBufferTop();
            velocityBufferBottom = velocityBufferFillStencil.getVelocityBufferBottom();

            MPI_Sendrecv(&velocityBufferTop[0], (int) velocityBufferTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                         &velocityBufferBottom[0], (int) velocityBufferBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.bottomNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferBottomIterator(velocityBufferBottom);
            }
        }

        /**
         * Communication: Front & Back
         */
        if (parameters_.parallel.frontNb != MPI_PROC_NULL || parameters_.parallel.backNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to front, receive from back
             */

            std::vector<FLOAT> velocityBufferFront = velocityBufferFillStencil.getVelocityBufferFront();
            std::vector<FLOAT> velocityBufferBack = velocityBufferFillStencil.getVelocityBufferBack();

            MPI_Sendrecv(&velocityBufferFront[0], (int) velocityBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                         &velocityBufferBack[0], (int) velocityBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.backNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferBackIterator(velocityBufferBack);
            }

            /**
             * Communication: Send to back, receive from front
             */

            velocityBufferFront = velocityBufferFillStencil.getVelocityBufferFront();
            velocityBufferBack = velocityBufferFillStencil.getVelocityBufferBack();

            MPI_Sendrecv(&velocityBufferBack[0], (int) velocityBufferBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                         &velocityBufferFront[0], (int) velocityBufferFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.frontNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferFrontIterator(velocityBufferFront);
            }
        }

        velocityBufferFillStencil.clearVelocityBuffers();
    }

} // namespace NSEOF::ParallelManagers
