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

            std::vector<FLOAT> SendLeft = pressureBufferFillStencil.getPressureBufferLeft();
            std::vector<FLOAT> RecvRight = pressureBufferFillStencil.getPressureBufferRight();

            MPI_Sendrecv(&SendLeft[0], (int) SendLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                         &RecvRight[0], (int) RecvRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.rightNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferRightIterator(RecvRight);
            }

            /**
             * Communication: Send to right, receive from left
             */

            std::vector<FLOAT> SendRight = pressureBufferFillStencil.getPressureBufferRight();
            std::vector<FLOAT> RecvLeft = pressureBufferFillStencil.getPressureBufferLeft();        

            MPI_Sendrecv(&SendRight[0], (int) SendRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                         &RecvLeft[0], (int) RecvLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.leftNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferLeftIterator(RecvLeft);
            }
        }

        /**
         * Communication: Bottom & Top
         */
        if (parameters_.parallel.bottomNb != MPI_PROC_NULL || parameters_.parallel.topNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to bottom, receive from top
             */

            std::vector<FLOAT> SendBottom = pressureBufferFillStencil.getPressureBufferBottom();
            std::vector<FLOAT> RecvTop = pressureBufferFillStencil.getPressureBufferTop();

            MPI_Sendrecv(&SendBottom[0], (int) SendBottom.size(), MY_MPI_FLOAT,parameters_.parallel.bottomNb, 0,
                         &RecvTop[0], (int) RecvTop.size(), MY_MPI_FLOAT,parameters_.parallel.topNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.topNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferTopIterator(RecvTop);
            }

            /**
             * Communication: Send to top, receive from bottom
             */

            std::vector<FLOAT> SendTop = pressureBufferFillStencil.getPressureBufferTop();
            std::vector<FLOAT> RecvBottom = pressureBufferFillStencil.getPressureBufferBottom();

            MPI_Sendrecv(&SendTop[0], (int) SendTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                         &RecvBottom[0], (int) RecvBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.bottomNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferBottomIterator(RecvBottom);
            }
        }

        /**
         * Communication: Front & Back
         */
        if (parameters_.parallel.frontNb != MPI_PROC_NULL || parameters_.parallel.backNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to front, receive from back
             */

            std::vector<FLOAT> SendFront = pressureBufferFillStencil.getPressureBufferFront();
            std::vector<FLOAT> RecvBack = pressureBufferFillStencil.getPressureBufferBack();

            MPI_Sendrecv(&SendFront[0], (int) SendFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                         &RecvBack[0], (int) RecvBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.backNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferBackIterator(RecvBack);
            }

            /**
             * Communication: Send to back, receive from front
             */

            std::vector<FLOAT> SendBack = pressureBufferFillStencil.getPressureBufferBack();
            std::vector<FLOAT> RecvFront = pressureBufferFillStencil.getPressureBufferFront();

            MPI_Sendrecv(&SendBack[0], (int) SendBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                         &RecvFront[0], (int) RecvFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.frontNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferFrontIterator(RecvFront);
            }
        }

        pressureBufferFillStencil.clearPressureBuffers();
    }

    void PetscParallelManager::communicateVelocity(Stencils::VelocityBufferFillStencil& velocityBufferFillStencil,
                                                   Stencils::VelocityBufferReadStencil& velocityBufferReadStencil) const {
        /**
         * Communication: Left & Right
         */
        if (parameters_.parallel.rightNb != MPI_PROC_NULL || parameters_.parallel.leftNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to left, receive from right
             */

            std::vector<FLOAT> SendLeft = velocityBufferFillStencil.getVelocityBufferLeft();
            std::vector<FLOAT> RecvRight = velocityBufferFillStencil.getVelocityBufferRight();

            MPI_Sendrecv(&SendLeft[0], (int) SendLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                         &RecvRight[0], (int) RecvRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.rightNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferRightIterator(RecvRight);
            }

            /**
             * Communication: Send to right, receive from left
             */

            std::vector<FLOAT> SendRight = velocityBufferFillStencil.getVelocityBufferRight();
            std::vector<FLOAT> RecvLeft = velocityBufferFillStencil.getVelocityBufferLeft();

            MPI_Sendrecv(&SendRight[0], (int) SendRight.size(), MY_MPI_FLOAT, parameters_.parallel.rightNb, 0,
                         &RecvLeft[0], (int) RecvLeft.size(), MY_MPI_FLOAT, parameters_.parallel.leftNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.leftNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferLeftIterator(RecvLeft);
            }
        }

        /**
         * Communication: Bottom & Top
         */
        if (parameters_.parallel.bottomNb != MPI_PROC_NULL || parameters_.parallel.topNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to bottom, receive from top
             */

            std::vector<FLOAT> SendBottom = velocityBufferFillStencil.getVelocityBufferBottom();
            std::vector<FLOAT> RecvTop = velocityBufferFillStencil.getVelocityBufferTop();

            MPI_Sendrecv(&SendBottom[0], (int) SendBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                         &RecvTop[0], (int) RecvTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.topNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferTopIterator(RecvTop);
            }

            /**
             * Communication: Send to top, receive from bottom
             */

            std::vector<FLOAT> SendTop = velocityBufferFillStencil.getVelocityBufferTop();
            std::vector<FLOAT> RecvBottom = velocityBufferFillStencil.getVelocityBufferBottom();

            MPI_Sendrecv(&SendTop[0], (int) SendTop.size(), MY_MPI_FLOAT, parameters_.parallel.topNb, 0,
                         &RecvBottom[0], (int) RecvBottom.size(), MY_MPI_FLOAT, parameters_.parallel.bottomNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.bottomNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferBottomIterator(RecvBottom);
            }
        }

        /**
         * Communication: Front & Back
         */
        if (parameters_.parallel.frontNb != MPI_PROC_NULL || parameters_.parallel.backNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to front, receive from back
             */

            std::vector<FLOAT> SendFront = velocityBufferFillStencil.getVelocityBufferFront();
            std::vector<FLOAT> RecvBack = velocityBufferFillStencil.getVelocityBufferBack();

            MPI_Sendrecv(&SendFront[0], (int) SendFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                         &RecvBack[0], (int) RecvBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.backNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferBackIterator(RecvBack);
            }

            /**
             * Communication: Send to back, receive from front
             */

            std::vector<FLOAT> SendBack = velocityBufferFillStencil.getVelocityBufferBack();
            std::vector<FLOAT> RecvFront = velocityBufferFillStencil.getVelocityBufferFront();

            MPI_Sendrecv(&SendBack[0], (int) SendBack.size(), MY_MPI_FLOAT, parameters_.parallel.backNb, 0,
                         &RecvFront[0], (int) RecvFront.size(), MY_MPI_FLOAT, parameters_.parallel.frontNb, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (parameters_.parallel.frontNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferFrontIterator(RecvFront);
            }
        }

        velocityBufferFillStencil.clearVelocityBuffers();
    }

} // namespace NSEOF::ParallelManagers
