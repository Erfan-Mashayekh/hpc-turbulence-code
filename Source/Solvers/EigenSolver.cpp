#include "EigenSolver.hpp"

#define ROW_MAJOR_IND(i, j, k, sizeX, sizeY) (((k) * (sizeX) * (sizeY)) + ((j) * (sizeX)) + (i))
#define COLUMN_MAJOR_IND(i, j, k, sizeY, sizeZ) (((i) * (sizeY) * (sizeZ)) + ((j) * (sizeZ)) + (k))

namespace NSEOF::Solvers {

    Constants::Constants(FLOAT dxLeft, FLOAT dxRight, FLOAT dyBottom, FLOAT dyTop, FLOAT dzFront, FLOAT dzBack)
        : dxLeft(dxLeft), dxRight(dxRight), dyBottom(dyBottom), dyTop(dyTop), dzFront(dzFront), dzBack(dzBack) {}

    void EigenSolver::fillConstantsVector_() {
        constantsVector_.clear();
        constantsVector_.reserve(dim_);

        for (int i = 0; i < cellsX_; i++) {
            for (int j = 0; j < cellsY_; j++) {
                for (int k = 0; k < cellsZ_; k++) {
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

    void EigenSolver::computeMatrixBoundaryLeftOrRight2D_(BoundaryType boundaryType,
                                                          const unsigned int startIdx, const int direction) {
        const MatrixXd identityMatrix = MatrixXd::Identity(cellsY_ - 2, cellsY_ - 2);

        const MatrixXd diagMat = (boundaryType == DIRICHLET ? 1.0 : 0.5) * identityMatrix;
        const MatrixXd offDiagMat = (boundaryType == DIRICHLET ? -1.0 : 0.5) * identityMatrix;

        matA_.block(startIdx, startIdx, cellsY_ - 2, cellsY_ - 2) = diagMat;
        matA_.block(startIdx, startIdx + (direction * cellsY_), cellsY_ - 2, cellsY_ - 2) = offDiagMat;
    }

    void EigenSolver::computeMatrixBoundariesBottomAndTop2D_() {
        MatrixXd verticalWallMat = MatrixXd::Zero(cellsY_, cellsY_);

        verticalWallMat(0, 0) = parameters_.walls.typeBottom == DIRICHLET ? 1.0 : 0.5;
        verticalWallMat(0, 1) = parameters_.walls.typeBottom == DIRICHLET ? -1.0 : 0.5;

        verticalWallMat(cellsY_ - 1, cellsY_ - 1) = parameters_.walls.typeTop == DIRICHLET ? 1.0 : 0.5;
        verticalWallMat(cellsY_ - 1, cellsY_ - 2) = parameters_.walls.typeTop == DIRICHLET ? -1.0 : 0.5;

        for (int j = 1; j < cellsX_ - 1; j++) {
            matA_.block(j * cellsY_, j * cellsY_, cellsY_, cellsY_) = verticalWallMat;
        }
    }

    int EigenSolver::getObstacle_(const int i, const int j, const int k = 0,
                                  const int iStart = 1, const int jStart = 1, const int kStart = 1) const {
        const int cellIndexX = i - iStart + 2;
        const int cellIndexY = j - jStart + 2;
        const int cellIndexZ = (parameters_.geometry.dim == 3) * (k - kStart + 2);

        return flowField_.getFlags().getValue(cellIndexX, cellIndexY, cellIndexZ);
    }

    int EigenSolver::getSumObstacles_() const {
        return pow(2, parameters_.geometry.dim * 2 + 1) - 1;
    }

    void EigenSolver::computeStencilRowForFluidCell_(const int stencilRowLength, VectorXd& stencilRow,
                                                     const int i, const int j, const int k = 0) const {
        const Constants center = constantsVector_[COLUMN_MAJOR_IND(i, j, k, cellsY_, cellsZ_)];
        const Constants left   = constantsVector_[COLUMN_MAJOR_IND(i, j - 1, k, cellsY_, cellsZ_)];
        const Constants right  = constantsVector_[COLUMN_MAJOR_IND(i, j + 1, k, cellsY_, cellsZ_)];
        const Constants bottom = constantsVector_[COLUMN_MAJOR_IND(i - 1, j, k, cellsY_, cellsZ_)];
        const Constants top    = constantsVector_[COLUMN_MAJOR_IND(i + 1, j, k, cellsY_, cellsZ_)];

        if (parameters_.geometry.dim == 2) { // 2D
            /* Center */ stencilRow(stencilRowLength / 2    ) = -2.0 / (center.dxRight * center.dxLeft) -
                                                                 2.0 / (center.dyTop * center.dyBottom);
            /* Left   */ stencilRow(0                       ) =  2.0 / (left.dxLeft * (left.dxLeft + left.dxRight));
            /* Right  */ stencilRow(stencilRowLength - 1    ) =  2.0 / (right.dxRight * (right.dxLeft + right.dxRight));
            /* Bottom */ stencilRow(stencilRowLength / 2 - 1) =  2.0 / (bottom.dyBottom * (bottom.dyTop + bottom.dyBottom));
            /* Top    */ stencilRow(stencilRowLength / 2 + 1) =  2.0 / (top.dyTop * (top.dyTop + top.dyBottom));
        } else { // 3D
            const Constants front = constantsVector_[COLUMN_MAJOR_IND(i, j, k - 1, cellsY_, cellsZ_)];
            const Constants back  = constantsVector_[COLUMN_MAJOR_IND(i, j, k + 1, cellsY_, cellsZ_)];

            // TODO: Implement the 3D part
        }
    }

    void EigenSolver::computeStencilRowForObstacleCellWithFluidAround_(const int obstacle,
                                                                       const int stencilRowLength, VectorXd& stencilRow) const {
        const FLOAT leftObstacle   = (FLOAT) ((obstacle & OBSTACLE_LEFT)   == 0);
        const FLOAT rightObstacle  = (FLOAT) ((obstacle & OBSTACLE_RIGHT)  == 0);
        const FLOAT bottomObstacle = (FLOAT) ((obstacle & OBSTACLE_BOTTOM) == 0);
        const FLOAT topObstacle    = (FLOAT) ((obstacle & OBSTACLE_TOP)    == 0);

        if (parameters_.geometry.dim == 2) { // 2D
            /* Center */ stencilRow(stencilRowLength / 2    ) = -1.0 * (leftObstacle + rightObstacle + bottomObstacle + topObstacle);
            /* Left   */ stencilRow(0                       ) = leftObstacle;
            /* Right  */ stencilRow(stencilRowLength - 1    ) = rightObstacle;
            /* Bottom */ stencilRow(stencilRowLength / 2 - 1) = bottomObstacle;
            /* Top    */ stencilRow(stencilRowLength / 2 + 1) = topObstacle;
        } else { // 3D
            const FLOAT frontObstacle = (FLOAT) ((obstacle & OBSTACLE_FRONT) == 0);
            const FLOAT backObstacle  = (FLOAT) ((obstacle & OBSTACLE_BACK)  == 0);

            // TODO: Implement the 3D part
        }
    }

    void EigenSolver::computeStencilRowForObstacleCell_(const int stencilRowLength, VectorXd& stencilRow) const {
        if (parameters_.geometry.dim == 2) { // 2D
            /* Center */ stencilRow(stencilRowLength / 2    ) = 1.0;
            /* Left   */ stencilRow(0                       ) = 0.0;
            /* Right  */ stencilRow(stencilRowLength - 1    ) = 0.0;
            /* Bottom */ stencilRow(stencilRowLength / 2 - 1) = 0.0;
            /* Top    */ stencilRow(stencilRowLength / 2 + 1) = 0.0;
        } else { // 3D
            // TODO: Implement the 3D part
        }
    }

    void EigenSolver::computeMatrix2D_() {
        /**
         * Fill the matrix on boundary conditions
         */

        computeMatrixBoundaryLeftOrRight2D_(parameters_.walls.typeLeft, 1, 1); // Left wall
        computeMatrixBoundaryLeftOrRight2D_(parameters_.walls.typeRight, (cellsX_ - 1) * cellsY_ + 1, -1); // Right wall

        computeMatrixBoundariesBottomAndTop2D_(); // Bottom and top walls

        /**
         * Fill the matrix with actual values
         */

        int row = cellsY_ + 1;
        int column = 1;

        // If 3D, use the actual bounds, otherwise, use [0, 1) and iterate on k only once!
        const int kLowerBound = (parameters_.geometry.dim == 3) ? 1 : 0;
        const int kUpperBound = (parameters_.geometry.dim == 3) ? (cellsZ_ - 1) : 1;

        for (int i = 1; i < cellsX_ - 1; i++, row += 2, column += 2) {
            for (int j = 1; j < cellsY_ - 1; j++, row++, column++) {
                for (int k = kLowerBound; k < kUpperBound; k++) {
                    const int obstacle = getObstacle_(i, j);

                    const int stencilRowLength = cellsY_ * 2 + 1;
                    VectorXd stencilRow = VectorXd::Zero(stencilRowLength);

                    if ((obstacle & OBSTACLE_SELF) == 0) { // It is a fluid cell
                        computeStencilRowForFluidCell_(stencilRowLength, stencilRow, i, j, k);
                    } else if (obstacle != getSumObstacles_()) { // Not a fluid cell, but fluid is somewhere around
                        computeStencilRowForObstacleCellWithFluidAround_(obstacle, stencilRowLength, stencilRow);
                    } else { // The cell is an obstacle cell surrounded by more obstacle cells
                        computeStencilRowForObstacleCell_(stencilRowLength, stencilRow);
                    }

                    matA_.block(row, column, 1, stencilRowLength) = stencilRow.transpose();
                }
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

    void EigenSolver::computeRHS_() {
        if (parameters_.geometry.dim == 2) { // 2D
            for (int i = 1; i < cellsX_ - 1; i++) {
                for (int j = 1; j < cellsY_ - 1; j++) {
                    rhs_(COLUMN_MAJOR_IND(i, j, 0, cellsY_, cellsZ_)) = flowField_.getRHS().getScalar(i + 1, j + 1);
                }
            }
        } else { // 3D
            for (int i = 1; i < cellsX_ - 1; i++) {
                for (int j = 1; j < cellsY_ - 1; j++) {
                    for (int k = 1; k < cellsZ_ - 1; k++) {
                        rhs_(COLUMN_MAJOR_IND(i, j, k, cellsY_, cellsZ_)) = flowField_.getRHS().getScalar(i + 1, j + 1, k + 1);
                    }
                }
            }
        }
    }

    void EigenSolver::solve() {
        computeRHS_();
        x_ = solver_.solve(rhs_);

        std::cout << "# of iterations: " << solver_.iterations() << std::endl;
        std::cout << "estimated error: " << solver_.error()      << std::endl;

        if (parameters_.geometry.dim == 2) { // 2D
            for (int i = 0; i < cellsX_; i++) {
                for (int j = 0; j < cellsY_; j++) {
                    flowField_.getPressure().getScalar(i + 1, j + 1) = x_(COLUMN_MAJOR_IND(i, j, 0, cellsY_, cellsZ_));
                }
            }
        } else { // 3D
            for (int i = 0; i < cellsX_; i++) {
                for (int j = 0; j < cellsY_; j++) {
                    for (int k = 0; k < cellsZ_; k++) {
                        flowField_.getPressure().getScalar(i + 1, j + 1, k + 1) = x_(COLUMN_MAJOR_IND(i, j, k, cellsY_, cellsZ_));
                    }
                }
            }
        }
    }

    inline void EigenSolver::reInitMatrix() {
        initMatrix_();
    }
} // namespace Solvers::NSEOF

