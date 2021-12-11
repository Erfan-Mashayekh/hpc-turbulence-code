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

    void PetscParallelManager::communicate(Stencils::BufferFillStencil& bufferFillStencil,
                                           Stencils::BufferReadStencil& bufferReadStencil) const {
        /**
         * Communication: Left & Right
         */
        if (parameters_.parallel.leftNb != MPI_PROC_NULL || parameters_.parallel.rightNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to left, receive from right
             */

            std::vector<FLOAT> bufferSentToLeft = bufferFillStencil.getBufferLeft();
            std::vector<FLOAT> bufferReceivedFromRight = bufferFillStencil.getBufferRight();

            sendRecvBuffers(bufferSentToLeft, parameters_.parallel.leftNb,
                            bufferReceivedFromRight, parameters_.parallel.rightNb);

            if (parameters_.parallel.rightNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferRightIterator(bufferReceivedFromRight);
            }

            /**
             * Communication: Send to right, receive from left
             */

            std::vector<FLOAT> bufferSentToRight = bufferFillStencil.getBufferRight();
            std::vector<FLOAT> bufferReceivedFromLeft = bufferFillStencil.getBufferLeft();

            sendRecvBuffers(bufferSentToRight, parameters_.parallel.rightNb,
                            bufferReceivedFromLeft, parameters_.parallel.leftNb);

            if (parameters_.parallel.leftNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferLeftIterator(bufferReceivedFromLeft);
            }
        }

        /**
         * Communication: Bottom & Top
         */
        if (parameters_.parallel.bottomNb != MPI_PROC_NULL || parameters_.parallel.topNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to bottom, receive from top
             */

            std::vector<FLOAT> bufferSentToBottom = bufferFillStencil.getBufferBottom();
            std::vector<FLOAT> bufferReceivedFromTop = bufferFillStencil.getBufferTop();

            sendRecvBuffers(bufferSentToBottom, parameters_.parallel.bottomNb,
                            bufferReceivedFromTop, parameters_.parallel.topNb);

            if (parameters_.parallel.topNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferTopIterator(bufferReceivedFromTop);
            }

            /**
             * Communication: Send to top, receive from bottom
             */

            std::vector<FLOAT> bufferSentToTop = bufferFillStencil.getBufferTop();
            std::vector<FLOAT> bufferReceivedFromBottom = bufferFillStencil.getBufferBottom();

            sendRecvBuffers(bufferSentToTop, parameters_.parallel.topNb,
                            bufferReceivedFromBottom, parameters_.parallel.bottomNb);

            if (parameters_.parallel.bottomNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferBottomIterator(bufferReceivedFromBottom);
            }
        }

        /**
         * Communication: Front & Back
         */
        if (parameters_.parallel.frontNb != MPI_PROC_NULL || parameters_.parallel.backNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to front, receive from back
             */

            std::vector<FLOAT> bufferSentToFront = bufferFillStencil.getBufferFront();
            std::vector<FLOAT> bufferReceivedFromBack = bufferFillStencil.getBufferBack();

            sendRecvBuffers(bufferSentToFront, parameters_.parallel.frontNb,
                            bufferReceivedFromBack, parameters_.parallel.backNb);

            if (parameters_.parallel.backNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferBackIterator(bufferReceivedFromBack);
            }

            /**
             * Communication: Send to back, receive from front
             */

            std::vector<FLOAT> bufferSentToBack = bufferFillStencil.getBufferBack();
            std::vector<FLOAT> bufferReceivedFromFront = bufferFillStencil.getBufferFront();

            sendRecvBuffers(bufferSentToBack, parameters_.parallel.backNb,
                            bufferReceivedFromFront, parameters_.parallel.frontNb);

            if (parameters_.parallel.frontNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferFrontIterator(bufferReceivedFromFront);
            }
        }

        bufferFillStencil.clearBuffers();
    }

    void PetscParallelManager::communicatePressure(Stencils::PressureBufferFillStencil& pressureBufferFillStencil,
                                                   Stencils::PressureBufferReadStencil& pressureBufferReadStencil) const {
        communicate(pressureBufferFillStencil, pressureBufferReadStencil);
    }

    void PetscParallelManager::communicateVelocity(Stencils::VelocityBufferFillStencil& velocityBufferFillStencil,
                                                   Stencils::VelocityBufferReadStencil& velocityBufferReadStencil) const {
        communicate(velocityBufferFillStencil, velocityBufferReadStencil);
    }

} // namespace NSEOF::ParallelManagers
