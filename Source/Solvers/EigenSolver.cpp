#include "EigenSolver.hpp"

#define ROW_MAJOR_IND(row, col, width) ((row) * (width) + (col))
#define COLUMN_MAJOR_IND(row, col, height) ((col) * (height) + (row))

namespace NSEOF::Solvers {

    Constants::Constants(FLOAT L, FLOAT R, FLOAT Bo, FLOAT T) : L(L), R(R), Bo(Bo), T(T) {}

    void EigenSolver::fillConstantsVector_() {
        constantsVector_.clear();
        constantsVector_.reserve(dim_);

        for (int i = 0; i < sizeX_; i++) {
            for (int j = 0; j < sizeY_; j++) {
                FLOAT dx_0 = parameters_.meshsize->getDx(i, j);
                FLOAT dx_M1 = parameters_.meshsize->getDx(i - 1, j);
                FLOAT dx_P1 = parameters_.meshsize->getDx(i + 1, j);
                FLOAT dy_0 = parameters_.meshsize->getDy(i, j);
                FLOAT dy_M1 = parameters_.meshsize->getDy(i, j - 1);
                FLOAT dy_P1 = parameters_.meshsize->getDy(i, j + 1);

                FLOAT dx_L = 0.5 * (dx_0 + dx_M1);
                FLOAT dx_R = 0.5 * (dx_0 + dx_P1);
                FLOAT dy_Bo = 0.5 * (dy_0 + dy_M1);
                FLOAT dy_T = 0.5 * (dy_0 + dy_P1);

                constantsVector_.emplace_back(dx_L, dx_R, dy_Bo, dy_T);
            }
        }
    }

    void EigenSolver::computeMatrixBoundaryLeftOrRight2D_(BoundaryType boundaryType,
                                                          const unsigned int startIdx, const int direction) {
        const MatrixXd identityMatrix = MatrixXd::Identity(sizeY_ - 2, sizeY_ - 2);

        const MatrixXd diagMat = (boundaryType == DIRICHLET ? 1.0 : 0.5) * identityMatrix;
        const MatrixXd offDiagMat = (boundaryType == DIRICHLET ? -1.0 : 0.5) * identityMatrix;

        matA_.block(startIdx, startIdx, sizeY_ - 2, sizeY_ - 2) = diagMat;
        matA_.block(startIdx, startIdx + (direction * sizeY_), sizeY_ - 2, sizeY_ - 2) = offDiagMat;
    }

    void EigenSolver::computeMatrixBoundariesBottomAndTop2D_() {
        MatrixXd verticalWallMat = MatrixXd::Zero(sizeY_, sizeY_);

        verticalWallMat(0, 0) = parameters_.walls.typeBottom == DIRICHLET ? 1.0 : 0.5;
        verticalWallMat(0, 1) = parameters_.walls.typeBottom == DIRICHLET ? -1.0 : 0.5;

        verticalWallMat(sizeY_ - 1, sizeY_ - 1) = parameters_.walls.typeTop == DIRICHLET ? 1.0 : 0.5;
        verticalWallMat(sizeY_ - 1, sizeY_ - 2) = parameters_.walls.typeTop == DIRICHLET ? -1.0 : 0.5;

        for (int j = 1; j < sizeY_ - 1; j++) {
            matA_.block(j * sizeY_, j * sizeY_, sizeY_, sizeY_) = verticalWallMat;
        }
    }

    int EigenSolver::getObstacle_(const int i, const int j, const int iLower = 1, const int jLower = 1) const {
        const int cellIndexX = i - iLower + 2;
        const int cellIndexY = j - jLower + 2;

        return flowField_.getFlags().getValue(cellIndexX, cellIndexY);
    }

    void EigenSolver::computeStencilRowForFluidCell_(const int i, const int j, const int stencilRowLength, VectorXd& stencilRow) const {
        const Constants centerDx = constantsVector_[COLUMN_MAJOR_IND(j, i, sizeY_)];
        const Constants leftDx   = constantsVector_[COLUMN_MAJOR_IND(j, i - 1, sizeY_)];
        const Constants rightDx  = constantsVector_[COLUMN_MAJOR_IND(j, i + 1, sizeY_)];
        const Constants bottomDx = constantsVector_[COLUMN_MAJOR_IND(j - 1, i, sizeY_)];
        const Constants topDx    = constantsVector_[COLUMN_MAJOR_IND(j + 1, i, sizeY_)];

        /* Center */ stencilRow(stencilRowLength / 2    ) = -2.0 / (centerDx.R * centerDx.L) - 2.0 / (centerDx.T * centerDx.Bo);
        /* Left   */ stencilRow(0                       ) =  2.0 / (leftDx.L * (leftDx.L + leftDx.R));
        /* Right  */ stencilRow(stencilRowLength - 1    ) =  2.0 / (rightDx.R * (rightDx.L + rightDx.R));
        /* Bottom */ stencilRow(stencilRowLength / 2 - 1) =  2.0 / (bottomDx.Bo * (bottomDx.T + bottomDx.Bo));
        /* Top    */ stencilRow(stencilRowLength / 2 + 1) =  2.0 / (topDx.T * (topDx.T + topDx.Bo));
    }

    void EigenSolver::computeStencilRowForObstacleCellWithFluidAround_(const int obstacle, const int stencilRowLength, VectorXd& stencilRow) const {
        const FLOAT left   = (FLOAT) ((obstacle & OBSTACLE_LEFT)   == 0);
        const FLOAT right  = (FLOAT) ((obstacle & OBSTACLE_RIGHT)  == 0);
        const FLOAT bottom = (FLOAT) ((obstacle & OBSTACLE_BOTTOM) == 0);
        const FLOAT top    = (FLOAT) ((obstacle & OBSTACLE_TOP)    == 0);

        /* Center */ stencilRow(stencilRowLength / 2    ) = left - right - bottom - top;
        /* Left   */ stencilRow(0                       ) = left;
        /* Right  */ stencilRow(stencilRowLength - 1    ) = right;
        /* Bottom */ stencilRow(stencilRowLength / 2 - 1) = bottom;
        /* Top    */ stencilRow(stencilRowLength / 2 + 1) = top;
    }

    void EigenSolver::computeStencilRowForObstacleCell_(const int stencilRowLength, VectorXd& stencilRow) const {
        /* Center */ stencilRow(stencilRowLength / 2    ) = 1.0;
        /* Left   */ stencilRow(0                       ) = 0.0;
        /* Right  */ stencilRow(stencilRowLength - 1    ) = 0.0;
        /* Bottom */ stencilRow(stencilRowLength / 2 - 1) = 0.0;
        /* Top    */ stencilRow(stencilRowLength / 2 + 1) = 0.0;
    }

    void EigenSolver::computeMatrix2D_() {
        /**
         * Fill the matrix on boundary conditions
         */

        computeMatrixBoundaryLeftOrRight2D_(parameters_.walls.typeLeft, 1, 1); // Left wall
        computeMatrixBoundaryLeftOrRight2D_(parameters_.walls.typeRight, (sizeX_ - 1) * sizeY_ + 1, -1); // Right wall

        computeMatrixBoundariesBottomAndTop2D_(); // Bottom and top walls

        /**
         * Fill the matrix with actual values
         */

        int row = sizeY_ + 1;
        int column = 1;

        for (int i = 1; i < sizeX_ - 1; i++, row += 2, column += 2) {
            for (int j = 1; j < sizeY_ - 1; j++, row++, column++) {
                const int obstacle = getObstacle_(i, j);

                const int stencilRowLength = sizeY_ * 2 + 1;
                VectorXd stencilRow = VectorXd::Zero(stencilRowLength);

                if ((obstacle & OBSTACLE_SELF) == 0) { // It is a fluid cell
                    computeStencilRowForFluidCell_(i, j, stencilRowLength, stencilRow);
                } else if (obstacle != OBSTACLE_SELF + OBSTACLE_LEFT + OBSTACLE_RIGHT + OBSTACLE_TOP + OBSTACLE_BOTTOM) { // Not a fluid cell, but fluid is somewhere around
                    computeStencilRowForObstacleCellWithFluidAround_(obstacle, stencilRowLength, stencilRow);
                } else { // The cell is an obstacle cell surrounded by more obstacle cells
                    computeStencilRowForObstacleCell_(stencilRowLength, stencilRow);
                }

                matA_.block(row, column, 1, stencilRowLength) = stencilRow.transpose();
            }
        }

        sparseMatA_ = matA_.sparseView();
    }

    void EigenSolver::initMatrix_() {
        matA_ = MatrixXd::Zero(dim_, dim_);
        rhs_ = VectorXd::Zero(dim_);
        x_ = VectorXd::Zero(dim_);

        fillConstantsVector_();
        computeMatrix2D_();

#if SOLVER_MAX_NUM_ITERATIONS != -1
        solver_.setMaxIterations(SOLVER_MAX_NUM_ITERATIONS);
#endif

        solver_.compute(sparseMatA_);
    }

    EigenSolver::EigenSolver(FlowField& flowField, Parameters& parameters)
        : LinearSolver(flowField, parameters)
        , flowField_(flowField)
        , parameters_(parameters)
        , sizeX_(parameters.parallel.localSize[0] + 2)
        , sizeY_(parameters_.parallel.localSize[1] + 2)
        , dim_(sizeX_ * sizeY_) {
        initMatrix_();
    }

    EigenSolver::~EigenSolver() {
        constantsVector_.clear();

        matA_.resize(0, 0);
        rhs_.resize(0);
        x_.resize(0);
    }

    void EigenSolver::computeRHS2D_() {
        for (int i = 1; i < sizeX_ - 1; i++) {
            for (int j = 1; j < sizeY_ - 1; j++) {
                rhs_(COLUMN_MAJOR_IND(j, i, sizeY_)) = flowField_.getRHS().getScalar(i + 1, j + 1);
            }
        }
    }

    void EigenSolver::solve() {
        computeRHS2D_();
        x_ = solver_.solve(rhs_);

        std::cout << "# of iterations: " << solver_.iterations() << std::endl;
        std::cout << "estimated error: " << solver_.error()      << std::endl;

        for (int i = 0; i < sizeX_; i++) {
            for (int j = 0; j < sizeY_; j++) {
                flowField_.getPressure().getScalar(i + 1, j + 1) = x_(COLUMN_MAJOR_IND(j, i, sizeY_));
            }
        }
    }

    inline void EigenSolver::reInitMatrix() {
        initMatrix_();
    }
} // namespace Solvers::NSEOF

