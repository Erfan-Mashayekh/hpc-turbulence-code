#include "PetscParallelManager.hpp"

namespace NSEOF::ParallelManagers {

    PetscParallelManager::PetscParallelManager(const Parameters& parameters, FlowField& flowField)
        : parameters_(parameters)
        , pressureBufferFillStencil_(parameters)
        , pressureBufferReadStencil_(parameters)
        , velocityBufferFillStencil_(parameters)
        , velocityBufferReadStencil_(parameters)
        , velocityBufferDiagonalFillStencil_(parameters)
        , velocityBufferDiagonalReadStencil_(parameters)
        , pressureBufferFillIterator_(flowField, parameters, pressureBufferFillStencil_,
                                      parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
        , pressureBufferReadIterator_(flowField, parameters, pressureBufferReadStencil_,
                                      parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
        , velocityBufferFillIterator_(flowField, parameters, velocityBufferFillStencil_,
                                      parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
        , velocityBufferReadIterator_(flowField, parameters, velocityBufferReadStencil_,
                                      parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
        , velocityBufferFillDiagonalIterator_(flowField, parameters, velocityBufferDiagonalFillStencil_,
                                              parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset)
        , velocityBufferReadDiagonalIterator_(flowField, parameters, velocityBufferDiagonalReadStencil_,
                                              parameters.vtk.whiteRegionLowOffset, parameters.vtk.whiteRegionHighOffset) {}

    void PetscParallelManager::sendRecvBuffers(std::vector<FLOAT>& bufferSent, int receiverRank,
                                               std::vector<FLOAT>& bufferReceived, int senderRank) {
        MPI_Sendrecv(&bufferSent[0],     (int) bufferSent.size(),     MY_MPI_FLOAT, receiverRank, 0,
                     &bufferReceived[0], (int) bufferReceived.size(), MY_MPI_FLOAT, senderRank,   0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    void PetscParallelManager::communicate_(Stencils::BufferFillStencil& bufferFillStencil,
                                            Stencils::BufferReadStencil& bufferReadStencil) const {
        std::vector<FLOAT> bufferLeft, bufferRight;
        std::vector<FLOAT> bufferBottom, bufferTop;
        std::vector<FLOAT> bufferFront, bufferBack;

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

    void PetscParallelManager::communicatePressure() {
        pressureBufferFillIterator_.iterate();
        communicate_(pressureBufferFillStencil_, pressureBufferReadStencil_);
        pressureBufferReadIterator_.iterate();
    }

    void PetscParallelManager::communicateVelocity() {
        velocityBufferFillIterator_.iterate();
        communicate_(velocityBufferFillStencil_, velocityBufferReadStencil_);
        velocityBufferReadIterator_.iterate();
    }

    void PetscParallelManager::communicateDiagonal_(Stencils::BufferFillStencil& bufferFillStencil,
                                                    Stencils::BufferReadStencil& bufferReadStencil) const {
        // Wait for all normal communications to end before communicating diagonally!
        MPI_Barrier(MPI_COMM_WORLD);

        std::vector<FLOAT> bufferLeft, bufferRight;
        std::vector<FLOAT> bufferBottom, bufferTop;
        std::vector<FLOAT> bufferFront, bufferBack;

        /**
         * Communication: Left-top & Right-bottom
         */
        if (parameters_.parallel.leftTopNb != MPI_PROC_NULL || parameters_.parallel.rightBottomNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to left-top, receive from right-bottom
             */

            bufferLeft = bufferFillStencil.getBufferLeft();
            bufferRight = bufferFillStencil.getBufferRight();

            sendRecvBuffers(bufferLeft, parameters_.parallel.leftTopNb,
                            bufferRight, parameters_.parallel.rightBottomNb);

            if (parameters_.parallel.rightBottomNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferRightIterator(bufferRight);
            }

            /**
             * Communication: Send to right-bottom, receive from left-top
             */

            bufferLeft = bufferFillStencil.getBufferLeft();
            bufferRight = bufferFillStencil.getBufferRight();

            sendRecvBuffers(bufferRight, parameters_.parallel.rightBottomNb,
                            bufferLeft, parameters_.parallel.leftTopNb);

            if (parameters_.parallel.leftTopNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferLeftIterator(bufferLeft);
            }
        }

        /**
         * Communication: Left-bottom & Right-top
         */
        if (parameters_.parallel.leftBottomNb != MPI_PROC_NULL || parameters_.parallel.rightTopNb != MPI_PROC_NULL) {
            /**
             * Communication: Send to left-bottom, receive from right-top
             */

            bufferBottom = bufferFillStencil.getBufferBottom();
            bufferTop = bufferFillStencil.getBufferTop();

            sendRecvBuffers(bufferBottom, parameters_.parallel.leftBottomNb,
                            bufferTop, parameters_.parallel.rightTopNb);

            if (parameters_.parallel.rightTopNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferTopIterator(bufferTop);
            }

            /**
             * Communication: Send to right-top, receive from left-bottom
             */

            bufferBottom = bufferFillStencil.getBufferBottom();
            bufferTop = bufferFillStencil.getBufferTop();

            sendRecvBuffers(bufferTop, parameters_.parallel.rightTopNb,
                            bufferBottom, parameters_.parallel.leftBottomNb);

            if (parameters_.parallel.leftBottomNb != MPI_PROC_NULL) {
                bufferReadStencil.setBufferBottomIterator(bufferBottom);
            }
        }

        bufferFillStencil.clearBuffers();
    }

    void PetscParallelManager::communicateDiagonalVelocity() {
        velocityBufferFillDiagonalIterator_.iterate();
        communicateDiagonal_(velocityBufferDiagonalFillStencil_, velocityBufferDiagonalReadStencil_);
        velocityBufferReadDiagonalIterator_.iterate();
    }

} // namespace NSEOF::ParallelManagers
