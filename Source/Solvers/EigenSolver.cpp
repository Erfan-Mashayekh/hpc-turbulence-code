#include "EigenSolver.hpp"

#define ROW_MAJOR_IND(row, col, width) ((row) * (width) + (col))
#define COLUMN_MAJOR_IND(row, col, height) ((col) * (height) + (row))

namespace NSEOF::Solvers {

    Constants::Constants(FLOAT L, FLOAT R, FLOAT Bo, FLOAT T) : L(L), R(R), Bo(Bo), T(T) {}

    void EigenSolver::fillConstantsVector() {
        constantsVector_.reserve(dim_);

        for (int j = 0; j < sizeY_; j++) {
            for (int i = 0; i < sizeX_; i++) {
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

    void EigenSolver::computeMatrixBoundaryLeftOrRight2D(BoundaryType boundaryType,
                                                         const unsigned int startIdx, const int direction) {
        const MatrixXd identityMatrix = MatrixXd::Identity(sizeY_ - 2, sizeY_ - 2);

        const MatrixXd diagMat = (boundaryType == DIRICHLET ? 1.0 : 0.5) * identityMatrix;
        const MatrixXd offDiagMat = (boundaryType == DIRICHLET ? -1.0 : 0.5) * identityMatrix;

        matA_.block(startIdx, startIdx, sizeY_ - 2, sizeY_ - 2) = diagMat;
        matA_.block(startIdx, startIdx + (direction * sizeY_), sizeY_ - 2, sizeY_ - 2) = offDiagMat;
    }

    void EigenSolver::computeMatrixBoundariesBottomAndTop2D() {
        MatrixXd verticalWallMat = MatrixXd::Identity(sizeY_, sizeY_);
        verticalWallMat *= (parameters_.walls.typeLeft == DIRICHLET ? 1.0 : 0.5);

        verticalWallMat(0, 1) = parameters_.walls.typeBottom == DIRICHLET ? -1.0 : 0.5;
        verticalWallMat(sizeY_ - 1, sizeY_ - 2) = parameters_.walls.typeTop == DIRICHLET ? -1.0 : 0.5;

        for (int i = sizeY_; i < dim_ - sizeY_; i += sizeY_) {
            matA_.block(i, i, sizeY_, sizeY_) = verticalWallMat;
        }
    }

    void EigenSolver::computeMatrix2D() {
        /**
         * Fill the matrix on boundary conditions
         */

        computeMatrixBoundaryLeftOrRight2D(parameters_.walls.typeLeft, 1, 1); // Left wall
        computeMatrixBoundaryLeftOrRight2D(parameters_.walls.typeRight, (sizeX_ - 1) * sizeY_ + 1, -1); // Right wall

        computeMatrixBoundariesBottomAndTop2D(); // Bottom and top walls

        /**
         * Fill the matrix with actual values
         */

        int row = sizeY_ + 1;
        int column = 1;

        for (int j = 1; j < sizeY_ - 1; j++, row += 2, column += 2) {
            for (int i = 1; i < sizeX_ - 1; i++, row++, column++) {
                const int vectorLength = sizeY_ * 2 + 1;
                VectorXd valueVector = VectorXd::Zero(vectorLength);

                const Constants centerDx = constantsVector_[ROW_MAJOR_IND(j, i, sizeX_)];
                const Constants leftDx   = constantsVector_[ROW_MAJOR_IND(j, i - 1, sizeX_)];
                const Constants rightDx  = constantsVector_[ROW_MAJOR_IND(j, i + 1, sizeX_)];
                const Constants bottomDx = constantsVector_[ROW_MAJOR_IND(j - 1, i, sizeX_)];
                const Constants topDx    = constantsVector_[ROW_MAJOR_IND(j + 1, i, sizeX_)];

                valueVector(vectorLength / 2) = -2.0 / (centerDx.R * centerDx.L) - 2.0 / (centerDx.T * centerDx.Bo); // Center
                valueVector(0) = 2.0 / (leftDx.L * (leftDx.L + leftDx.R)); // Left
                valueVector(vectorLength - 1) = 2.0 / (rightDx.R * (rightDx.L + rightDx.R)); // Right
                valueVector(vectorLength / 2 - 1) = 2.0 / (bottomDx.Bo * (bottomDx.T + bottomDx.Bo)); // Bottom
                valueVector(vectorLength / 2 + 1) = 2.0 / (topDx.T * (topDx.T + topDx.Bo)); // Top

                matA_.block(row, column, 1, vectorLength) = valueVector.transpose();
            }
        }

        sparseMatA_ = matA_.sparseView();

        // std::cout << matA_ << std::endl;
        // std::cout << sparseMatA_ << std::endl;
    }

    EigenSolver::EigenSolver(FlowField& flowField, Parameters& parameters)
        : LinearSolver(flowField, parameters)
        , flowField_(flowField)
        , parameters_(parameters)
        , sizeX_(parameters.parallel.localSize[0] + 2)
        , sizeY_(parameters_.parallel.localSize[1] + 2)
        , dim_(sizeX_ * sizeY_)
        , matA_(MatrixXd::Zero(dim_, dim_))
        , rhs_(VectorXd::Zero(dim_)) {
        fillConstantsVector();
        computeMatrix2D();
    }

    EigenSolver::~EigenSolver() {
        constantsVector_.clear();

        matA_.resize(0, 0);
        sparseMatA_.resize(0, 0);
        rhs_.resize(0, 0);
    }

    void EigenSolver::computeRHS2D() {
        for (int i = 1; i < sizeX_ - 1; i++) {
            for (int j = 1; j < sizeY_ - 1; j++) {
                rhs_(COLUMN_MAJOR_IND(j, i, sizeY_)) = flowField_.getRHS().getScalar(i + 1, j + 1);
            }
        }
    }

    void EigenSolver::solve() {
        computeRHS2D();

        VectorXd x(dim_);
        BiCGSTAB<SparseMatrix<FLOAT>> solver;

        solver.setMaxIterations(50);
        solver.compute(sparseMatA_);

        x = solver.solve(rhs_);

        std::cout << "#iterations:     " << solver.iterations() << std::endl;
        std::cout << "estimated error: " << solver.error()      << std::endl;

        for (int i = 0; i < sizeX_; i++) {
            for (int j = 0; j < sizeY_; j++) {
                flowField_.getPressure().getScalar(i + 1, j + 1) = x(COLUMN_MAJOR_IND(j, i, sizeY_));
            }
        }
    }

    inline void EigenSolver::reInitMatrix() {
        matA_ = MatrixXd::Zero(dim_, dim_);
        rhs_ = VectorXd::Zero(dim_);
    }
} // namespace Solvers::NSEOF

