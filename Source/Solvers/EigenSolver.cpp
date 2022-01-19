#include "EigenSolver.hpp"

#define ROW_MAJOR_IND(i, j, k, sizeX, sizeY) ((i) + ((j) * (sizeX)) + ((k) * (sizeX) * (sizeY)))
#define COLUMN_MAJOR_IND(i, j, k, sizeY, sizeZ) ((k) + ((j) * (sizeZ)) + ((i) * (sizeY) * (sizeZ)))

namespace NSEOF::Solvers {

    Constants::Constants(FLOAT dxLeft, FLOAT dxRight, FLOAT dyBottom, FLOAT dyTop, FLOAT dzFront, FLOAT dzBack)
        : dxLeft(dxLeft), dxRight(dxRight), dyBottom(dyBottom), dyTop(dyTop), dzFront(dzFront), dzBack(dzBack) {}

    void EigenSolver::fillConstantsVector_() {
        constantsVector_.clear();
        constantsVector_.reserve(dim_);

        for (int k = 0; k < cellsZ_; k++) {
            for (int j = 0; j < cellsY_; j++) {
                for (int i = 0; i < cellsX_; i++) {
                    const FLOAT dx = parameters_.meshsize->getDx(i, j, k);
                    const FLOAT dy = parameters_.meshsize->getDy(i, j, k);
                    const FLOAT dz = parameters_.meshsize->getDz(i, j, k);

                    const FLOAT dxLeft   = 0.5 * (dx + parameters_.meshsize->getDx(i - 1, j, k));
                    const FLOAT dxRight  = 0.5 * (dx + parameters_.meshsize->getDx(i + 1, j, k));
                    const FLOAT dyBottom = 0.5 * (dy + parameters_.meshsize->getDy(i, j - 1, k));
                    const FLOAT dyTop    = 0.5 * (dy + parameters_.meshsize->getDy(i, j + 1, k));
                    const FLOAT dzFront  = 0.5 * (dz + parameters_.meshsize->getDz(i, j, k - 1));
                    const FLOAT dzBack   = 0.5 * (dz + parameters_.meshsize->getDz(i, j, k + 1));

                    constantsVector_.emplace_back(dxLeft, dxRight, dyBottom, dyTop, dzFront, dzBack);
                }
            }
        }
    }

    void EigenSolver::computeStencilRowForFluidCell_(const int stencilRowLength, VectorXd& stencilRow,
                                                     const int i, const int j, const int k = 0) const {
        const Constants bottom = constantsVector_[ROW_MAJOR_IND(i - 1, j, k, cellsX_, cellsY_)];
        const Constants left   = constantsVector_[ROW_MAJOR_IND(i, j - 1, k, cellsX_, cellsY_)];
        const Constants center = constantsVector_[ROW_MAJOR_IND(i, j, k, cellsX_, cellsY_)];
        const Constants right  = constantsVector_[ROW_MAJOR_IND(i, j + 1, k, cellsX_, cellsY_)];
        const Constants top    = constantsVector_[ROW_MAJOR_IND(i + 1, j, k, cellsX_, cellsY_)];

        if (parameters_.geometry.dim == 2) { // 2D
            /* Bottom */ stencilRow(0                       ) =  2.0 / (bottom.dyBottom * (bottom.dyBottom + bottom.dyTop));
            /* Left   */ stencilRow(stencilRowLength / 2 - 1) =  2.0 / (left.dxLeft * (left.dxLeft + left.dxRight));
            /* Center */ stencilRow(stencilRowLength / 2    ) = -2.0 / (center.dxLeft * center.dxRight) - 2.0 / (center.dyBottom * center.dyTop);
            /* Right  */ stencilRow(stencilRowLength / 2 + 1) =  2.0 / (right.dxRight * (right.dxLeft + right.dxRight));
            /* Top    */ stencilRow(stencilRowLength - 1    ) =  2.0 / (top.dyTop * (top.dyBottom + top.dyTop));
        } else { // 3D
            const Constants front = constantsVector_[ROW_MAJOR_IND(i, j, k - 1, cellsX_, cellsY_)];
            const Constants back  = constantsVector_[ROW_MAJOR_IND(i, j, k + 1, cellsX_, cellsY_)];

            // TODO: Implement the 3D part
        }
    }

    void EigenSolver::computeStencilRowForObstacleCellWithFluidAround_(const int obstacle,
                                                                       const int stencilRowLength, VectorXd& stencilRow) const {
        const auto bottomObstacle = (FLOAT) ((obstacle & OBSTACLE_BOTTOM) == 0);
        const auto leftObstacle   = (FLOAT) ((obstacle & OBSTACLE_LEFT)   == 0);
        const auto rightObstacle  = (FLOAT) ((obstacle & OBSTACLE_RIGHT)  == 0);
        const auto topObstacle    = (FLOAT) ((obstacle & OBSTACLE_TOP)    == 0);

        if (parameters_.geometry.dim == 2) { // 2D
            /* Bottom */ stencilRow(0                       ) = bottomObstacle;
            /* Left   */ stencilRow(stencilRowLength / 2 - 1) = leftObstacle;
            /* Center */ stencilRow(stencilRowLength / 2    ) = -1.0 * (leftObstacle + rightObstacle + bottomObstacle + topObstacle);
            /* Right  */ stencilRow(stencilRowLength / 2 + 1) = rightObstacle;
            /* Top    */ stencilRow(stencilRowLength - 1    ) = topObstacle;
        } else { // 3D
            const auto frontObstacle = (FLOAT) ((obstacle & OBSTACLE_FRONT) == 0);
            const auto backObstacle  = (FLOAT) ((obstacle & OBSTACLE_BACK)  == 0);

            // TODO: Implement the 3D part
        }
    }

    void EigenSolver::computeStencilRowForObstacleCell_(const int stencilRowLength, VectorXd& stencilRow) const {
        if (parameters_.geometry.dim == 2) { // 2D
            /* Bottom */ stencilRow(0                       ) = 0.0;
            /* Left   */ stencilRow(stencilRowLength / 2 - 1) = 0.0;
            /* Center */ stencilRow(stencilRowLength / 2    ) = 1.0;
            /* Right  */ stencilRow(stencilRowLength / 2 + 1) = 0.0;
            /* Top    */ stencilRow(stencilRowLength - 1    ) = 0.0;
        } else { // 3D
            // TODO: Implement the 3D part
        }
    }

    void EigenSolver::computeMatrixOnWhiteRegion_() {
        const int sumObstacles = pow(2, parameters_.geometry.dim * 2 + 1) - 1;

        int row = cellsX_ + 1;
        int column = 1;

        // If 3D, use the actual bounds, otherwise, use [0, 1) and iterate on k only once!
        const int kLowerBound = (parameters_.geometry.dim == 3) ? 1 : 0;
        const int kUpperBound = (parameters_.geometry.dim == 3) ? (cellsZ_ - 1) : 1;

        for (int k = kLowerBound; k < kUpperBound; k++) {
            for (int j = 1; j < cellsY_ - 1; j++, row += 2, column += 2) {
                for (int i = 1; i < cellsX_ - 1; i++, row++, column++) {
                    const int obstacle = flowField_.getFlags().getValue(
                            i + 1, j + 1, (parameters_.geometry.dim == 3) * (k + 1));

                    const int stencilRowLength = cellsX_ * 2 + 1;
                    VectorXd stencilRow = VectorXd::Zero(stencilRowLength);

                    if ((obstacle & OBSTACLE_SELF) == 0) { // It is a fluid cell
                        computeStencilRowForFluidCell_(stencilRowLength, stencilRow, i, j, k);
                    } else if (obstacle != sumObstacles) { // Not a fluid cell, but fluid is somewhere around
                        computeStencilRowForObstacleCellWithFluidAround_(obstacle, stencilRowLength, stencilRow);
                    } else { // The cell is an obstacle cell surrounded by more obstacle cells
                        computeStencilRowForObstacleCell_(stencilRowLength, stencilRow);
                    }

                    matA_.block(row, column, 1, stencilRowLength) = stencilRow.transpose();
                }
            }
        }
    }

    void EigenSolver::computeMatrixOnBoundariesLeftAndRight_() {
        VectorXd leftWallVector(2), rightWallVector(2);

        leftWallVector << (parameters_.walls.typeLeft == DIRICHLET ? 1.0 : 0.5),
                          (parameters_.walls.typeLeft == DIRICHLET ? -1.0 : 0.5);
        rightWallVector << (parameters_.walls.typeRight == DIRICHLET ? -1.0 : 0.5),
                           (parameters_.walls.typeRight == DIRICHLET ? 1.0 : 0.5);

        for (int j = 1; j < cellsY_ - 1; j++) {
            matA_.block(j * cellsX_, j * cellsX_, 1, 2) = leftWallVector.transpose();
            matA_.block((j + 1) * cellsX_ - 1, (j + 1) * cellsX_ - 2, 1, 2) = rightWallVector.transpose();
        }
    }

    void EigenSolver::computeMatrixOnBoundaryBottomOrTop_(BoundaryType boundaryType,
                                                          const unsigned int startIdx, const int direction) {
        const MatrixXd identityMatrix = MatrixXd::Identity(cellsX_ - 2, cellsX_ - 2);

        const MatrixXd diagMat = (boundaryType == DIRICHLET ? 1.0 : 0.5) * identityMatrix;
        const MatrixXd offDiagMat = (boundaryType == DIRICHLET ? -1.0 : 0.5) * identityMatrix;

        matA_.block(startIdx, startIdx, cellsX_ - 2, cellsX_ - 2) = diagMat;
        matA_.block(startIdx, startIdx + (direction * cellsX_), cellsX_ - 2, cellsX_ - 2) = offDiagMat;
    }

    // TODO: Extend for 3D part
    void EigenSolver::computeMatrix_() {
        /**
         * Fill the matrix on white region
         */

        computeMatrixOnWhiteRegion_(); // White region

        /**
         * Fill the matrix on boundary conditions
         */

        computeMatrixOnBoundariesLeftAndRight_(); // Left and right walls

        computeMatrixOnBoundaryBottomOrTop_(parameters_.walls.typeBottom, 1, 1); // Bottom wall
        computeMatrixOnBoundaryBottomOrTop_(parameters_.walls.typeTop, (cellsY_ - 1) * cellsX_ + 1, -1); // Top wall

        /**
         * Convert the matrix to a sparse matrix
         */

        sparseMatA_ = matA_.sparseView();
    }

    void EigenSolver::initMatrix_() {
        matA_ = MatrixXd::Zero(dim_, dim_);
        rhs_ = VectorXd::Zero(dim_);
        x_ = VectorXd::Zero(dim_);

        fillConstantsVector_();
        computeMatrix_();

#if SOLVER_MAX_NUM_ITERATIONS != -1
        solver_.setMaxIterations(SOLVER_MAX_NUM_ITERATIONS);
#endif

        solver_.compute(sparseMatA_);
    }

    EigenSolver::EigenSolver(FlowField& flowField, const Parameters& parameters)
        : LinearSolver(flowField, parameters)
        , cellsX_(parameters.parallel.localSize[0] + 2)
        , cellsY_(parameters.parallel.localSize[1] + 2)
        , cellsZ_(parameters.geometry.dim == 3 ? (parameters.parallel.localSize[2] + 2) : 1)
        , dim_(cellsX_ * cellsY_ * cellsZ_) {
        initMatrix_();
    }

    EigenSolver::~EigenSolver() {
        constantsVector_.clear();

        matA_.resize(0, 0);
        rhs_.resize(0);
        x_.resize(0);
    }

    FLOAT EigenSolver::getScalarRHS_(const int obstacle, int i, int j, int k = 0) {
        if ((obstacle & OBSTACLE_SELF) == 0) { // Fluid cell
            return flowField_.getRHS().getScalar(i, j, k);
        }

        return 0.0; // Obstacle cell
    }

    void EigenSolver::computeRHS2D_() {
        for (int j = 1; j < cellsY_ - 1; j++) {
            for (int i = 1; i < cellsX_ - 1; i++) {
                const int obstacle = flowField_.getFlags().getValue(i + 1, j + 1);
                rhs_(ROW_MAJOR_IND(i, j, 0, cellsX_, cellsY_)) = getScalarRHS_(obstacle, i + 1, j + 1);
            }
        }
    }

    void EigenSolver::computeRHS3D_() {
        for (int k = 1; k < cellsZ_ - 1; k++) {
            for (int j = 1; j < cellsY_ - 1; j++) {
                for (int i = 1; i < cellsX_ - 1; i++) {
                    const int obstacle = flowField_.getFlags().getValue(i + 1, j + 1, k + 1);
                    rhs_(ROW_MAJOR_IND(i, j, k, cellsX_, cellsY_)) = getScalarRHS_(obstacle, i + 1, j + 1, k + 1);
                }
            }
        }
    }

    void EigenSolver::setPressure2D_() {
        for (int j = 0; j < cellsY_; j++) {
            for (int i = 0; i < cellsX_; i++) {
                flowField_.getPressure().getScalar(i + 1, j + 1) = x_(ROW_MAJOR_IND(i, j, 0, cellsX_, cellsY_));
            }
        }
    }

    void EigenSolver::setPressure3D_() {
        for (int k = 0; k < cellsZ_; k++) {
            for (int j = 0; j < cellsY_; j++) {
                for (int i = 0; i < cellsX_; i++) {
                    flowField_.getPressure().getScalar(i + 1, j + 1, k + 1) = x_(ROW_MAJOR_IND(i, j, k, cellsX_, cellsY_));
                }
            }
        }
    }

    void EigenSolver::solve() {
        // Compute RHS
        if (parameters_.geometry.dim == 2) { // 2D
            computeRHS2D_();
        } else { // 3D
            computeRHS3D_();
        }

        x_ = solver_.solve(rhs_);

        std::cout << "# of iterations: " << solver_.iterations() << std::endl;
        std::cout << "estimated error: " << solver_.error()      << std::endl;

        if (parameters_.geometry.dim == 2) { // 2D
            setPressure2D_();
        } else { // 3D
            setPressure3D_();
        }
    }

    inline void EigenSolver::reInitMatrix() {
        initMatrix_();
    }
} // namespace Solvers::NSEOF
