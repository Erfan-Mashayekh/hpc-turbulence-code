#include "PetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    PetscParallelManager::PetscParallelManager(const Parameters& parameters)
            : parameters_(parameters) {}

    void PetscParallelManager::sendRecvBuffers(std::vector<FLOAT>& bufferSent, int receiverRank,
                                               std::vector<FLOAT>& bufferReceived, int senderRank) {
        MPI_Sendrecv(&bufferSent[0], (int) bufferSent.size(), MY_MPI_FLOAT, receiverRank, 0,
                     &bufferReceived[0], (int) bufferReceived.size(), MY_MPI_FLOAT, senderRank, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    void PetscParallelManager::communicatePressure(Stencils::PressureBufferFillStencil& pressureBufferFillStencil,
                                                   Stencils::PressureBufferReadStencil& pressureBufferReadStencil) const {
        /**
         * Communication: Left & Right
         */
        if (parameters_.parallel.rightNb != MPI_PROC_NULL || parameters_.parallel.leftNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to left, receive from right
             */

            std::vector<FLOAT> bufferSentToLeft = pressureBufferFillStencil.getPressureBufferLeft();
            std::vector<FLOAT> bufferReceivedFromRight = pressureBufferFillStencil.getPressureBufferRight();

            sendRecvBuffers(bufferSentToLeft, parameters_.parallel.leftNb,
                            bufferReceivedFromRight, parameters_.parallel.rightNb);

            if (parameters_.parallel.rightNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferRightIterator(bufferReceivedFromRight);
            }

            /**
             * Communication: Send to right, receive from left
             */

            std::vector<FLOAT> bufferSentToRight = pressureBufferFillStencil.getPressureBufferRight();
            std::vector<FLOAT> bufferReceivedFromLeft = pressureBufferFillStencil.getPressureBufferLeft();

            sendRecvBuffers(bufferSentToRight, parameters_.parallel.rightNb,
                            bufferReceivedFromLeft, parameters_.parallel.leftNb);

            if (parameters_.parallel.leftNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferLeftIterator(bufferReceivedFromLeft);
            }
        }

        /**
         * Communication: Bottom & Top
         */
        if (parameters_.parallel.bottomNb != MPI_PROC_NULL || parameters_.parallel.topNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to bottom, receive from top
             */

            std::vector<FLOAT> bufferSentToBottom = pressureBufferFillStencil.getPressureBufferBottom();
            std::vector<FLOAT> bufferReceivedFromTop = pressureBufferFillStencil.getPressureBufferTop();

            sendRecvBuffers(bufferSentToBottom, parameters_.parallel.bottomNb,
                            bufferReceivedFromTop, parameters_.parallel.topNb);

            if (parameters_.parallel.topNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferTopIterator(bufferReceivedFromTop);
            }

            /**
             * Communication: Send to top, receive from bottom
             */

            std::vector<FLOAT> bufferSentToTop = pressureBufferFillStencil.getPressureBufferTop();
            std::vector<FLOAT> bufferReceivedFromBottom = pressureBufferFillStencil.getPressureBufferBottom();

            sendRecvBuffers(bufferSentToTop, parameters_.parallel.topNb,
                            bufferReceivedFromBottom, parameters_.parallel.bottomNb);

            if (parameters_.parallel.bottomNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferBottomIterator(bufferReceivedFromBottom);
            }
        }

        /**
         * Communication: Front & Back
         */
        if (parameters_.parallel.frontNb != MPI_PROC_NULL || parameters_.parallel.backNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to front, receive from back
             */

            std::vector<FLOAT> bufferSentToFront = pressureBufferFillStencil.getPressureBufferFront();
            std::vector<FLOAT> bufferReceivedFromBack = pressureBufferFillStencil.getPressureBufferBack();

            sendRecvBuffers(bufferSentToFront, parameters_.parallel.frontNb,
                            bufferReceivedFromBack, parameters_.parallel.backNb);

            if (parameters_.parallel.backNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferBackIterator(bufferReceivedFromBack);
            }

            /**
             * Communication: Send to back, receive from front
             */

            std::vector<FLOAT> bufferSentToBack = pressureBufferFillStencil.getPressureBufferBack();
            std::vector<FLOAT> bufferReceivedFromFront = pressureBufferFillStencil.getPressureBufferFront();

            sendRecvBuffers(bufferSentToBack, parameters_.parallel.backNb,
                            bufferReceivedFromFront, parameters_.parallel.frontNb);

            if (parameters_.parallel.frontNb != MPI_PROC_NULL) {
                pressureBufferReadStencil.setPressureBufferFrontIterator(bufferReceivedFromFront);
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

            std::vector<FLOAT> bufferSentToLeft = velocityBufferFillStencil.getVelocityBufferLeft();
            std::vector<FLOAT> bufferReceivedFromRight = velocityBufferFillStencil.getVelocityBufferRight();

            sendRecvBuffers(bufferSentToLeft, parameters_.parallel.leftNb,
                            bufferReceivedFromRight, parameters_.parallel.rightNb);

            if (parameters_.parallel.rightNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferRightIterator(bufferReceivedFromRight);
            }

            /**
             * Communication: Send to right, receive from left
             */

            std::vector<FLOAT> bufferSentToRight = velocityBufferFillStencil.getVelocityBufferRight();
            std::vector<FLOAT> bufferReceivedFromLeft = velocityBufferFillStencil.getVelocityBufferLeft();

            sendRecvBuffers(bufferSentToRight, parameters_.parallel.rightNb,
                            bufferReceivedFromLeft, parameters_.parallel.leftNb);

            if (parameters_.parallel.leftNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferLeftIterator(bufferReceivedFromLeft);
            }
        }

        /**
         * Communication: Bottom & Top
         */
        if (parameters_.parallel.bottomNb != MPI_PROC_NULL || parameters_.parallel.topNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to bottom, receive from top
             */

            std::vector<FLOAT> bufferSentToBottom = velocityBufferFillStencil.getVelocityBufferBottom();
            std::vector<FLOAT> bufferReceivedFromTop = velocityBufferFillStencil.getVelocityBufferTop();

            sendRecvBuffers(bufferSentToBottom, parameters_.parallel.bottomNb,
                            bufferReceivedFromTop, parameters_.parallel.topNb);

            if (parameters_.parallel.topNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferTopIterator(bufferReceivedFromTop);
            }

            /**
             * Communication: Send to top, receive from bottom
             */

            std::vector<FLOAT> bufferSentToTop = velocityBufferFillStencil.getVelocityBufferTop();
            std::vector<FLOAT> bufferReceivedFromBottom = velocityBufferFillStencil.getVelocityBufferBottom();

            sendRecvBuffers(bufferSentToTop, parameters_.parallel.topNb,
                            bufferReceivedFromBottom, parameters_.parallel.bottomNb);

            if (parameters_.parallel.bottomNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferBottomIterator(bufferReceivedFromBottom);
            }
        }

        /**
         * Communication: Front & Back
         */
        if (parameters_.parallel.frontNb != MPI_PROC_NULL || parameters_.parallel.backNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to front, receive from back
             */

            std::vector<FLOAT> bufferSentToFront = velocityBufferFillStencil.getVelocityBufferFront();
            std::vector<FLOAT> bufferReceivedFromBack = velocityBufferFillStencil.getVelocityBufferBack();

            sendRecvBuffers(bufferSentToFront, parameters_.parallel.frontNb,
                            bufferReceivedFromBack, parameters_.parallel.backNb);

            if (parameters_.parallel.backNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferBackIterator(bufferReceivedFromBack);
            }

            /**
             * Communication: Send to back, receive from front
             */

            std::vector<FLOAT> bufferSentToBack = velocityBufferFillStencil.getVelocityBufferBack();
            std::vector<FLOAT> bufferReceivedFromFront = velocityBufferFillStencil.getVelocityBufferFront();

            sendRecvBuffers(bufferSentToBack, parameters_.parallel.backNb,
                            bufferReceivedFromFront, parameters_.parallel.frontNb);

            if (parameters_.parallel.frontNb != MPI_PROC_NULL) {
                velocityBufferReadStencil.setVelocityBufferFrontIterator(bufferReceivedFromFront);
            }
        }

        velocityBufferFillStencil.clearVelocityBuffers();
    }

} // namespace NSEOF::ParallelManagers
