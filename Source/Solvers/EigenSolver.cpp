#include "EigenSolver.hpp"

#define ROW_MAJOR_IND(row, col, width) ((row) * (width) + (col))
#define COLUMN_MAJOR_IND(row, col, height) ((col) * (height) + (row))

namespace NSEOF::Solvers {

    DxConstants::DxConstants(FLOAT L, FLOAT R, FLOAT Bo, FLOAT T) : L(L), R(R), Bo(Bo), T(T) {}

    void EigenSolver::fillDxConstantsVector() {
        dxConstantsVector_.reserve(dim_);

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
                FLOAT dx_Bo = 0.5 * (dy_0 + dy_M1);
                FLOAT dx_T = 0.5 * (dy_0 + dy_P1);

                dxConstantsVector_.emplace_back(dx_L, dx_R, dx_Bo, dx_T);
            }
        }
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
        fillDxConstantsVector();
    }

    EigenSolver::~EigenSolver() {
        dxConstantsVector_.clear();
    }

    void EigenSolver::computeMatrix2D() {
        /**
         * Fill the matrix on boundary conditions
         */

        const MatrixXd identityMatrix = MatrixXd::Identity(sizeY_ - 2, sizeY_ - 2);
        unsigned int centerStartIdx;

        const MatrixXd diagMat = (parameters_.walls.typeLeft == DIRICHLET ? 1.0 : 0.5) * identityMatrix;
        const MatrixXd offDiagMat = (parameters_.walls.typeLeft == DIRICHLET ? -1.0 : 0.5) * identityMatrix;

        // Left wall
        centerStartIdx = 1;

        matA_.block(centerStartIdx, centerStartIdx, sizeY_ - 2, sizeY_ - 2) = diagMat;
        matA_.block(centerStartIdx, centerStartIdx + sizeY_, sizeY_ - 2, sizeY_ - 2) = offDiagMat;

        // Right wall
        centerStartIdx = (sizeX_ - 1) * sizeY_ + 1;

        matA_.block(centerStartIdx, centerStartIdx, sizeY_ - 2, sizeY_ - 2) = diagMat;
        matA_.block(centerStartIdx, centerStartIdx - sizeY_, sizeY_ - 2, sizeY_ - 2) = offDiagMat;

        // Bottom and top walls
        MatrixXd verticalWallMat = MatrixXd::Identity(sizeY_, sizeY_);
        verticalWallMat *= (parameters_.walls.typeLeft == DIRICHLET ? 1.0 : 0.5);

        verticalWallMat(0, 1) = parameters_.walls.typeLeft == DIRICHLET ? -1.0 : 0.5;
        verticalWallMat(sizeY_ - 1, sizeY_ - 2) = parameters_.walls.typeLeft == DIRICHLET ? -1.0 : 0.5;

        for (int i = sizeY_; i < dim_ - sizeY_; i += sizeY_) {
            matA_.block(i, i, sizeY_, sizeY_) = verticalWallMat;
        }

        /**
         * Fill the matrix with actual values
         */

        int row = sizeY_ + 1;
        int column = 1;

        for (int j = 1; j < sizeY_ - 1; j++, row += 2, column += 2) {
            for (int i = 1; i < sizeX_ - 1; i++, row++, column++) {
                const int vectorLength = sizeY_ * 2 + 1;
                VectorXd valueVector = VectorXd::Zero(vectorLength);

                const DxConstants centerDx = dxConstantsVector_[ROW_MAJOR_IND(j, i, sizeX_)];
                const DxConstants leftDx = dxConstantsVector_[ROW_MAJOR_IND(j, i - 1, sizeX_)];
                const DxConstants rightDx = dxConstantsVector_[ROW_MAJOR_IND(j, i + 1, sizeX_)];
                const DxConstants bottomDx = dxConstantsVector_[ROW_MAJOR_IND(j - 1, i, sizeX_)];
                const DxConstants topDx = dxConstantsVector_[ROW_MAJOR_IND(j + 1, i, sizeX_)];

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

    void EigenSolver::computeRHS2D() {
        for (int i = 1; i < sizeX_ - 1; i++) {
            for (int j = 1; j < sizeY_ - 1; j++) {
                rhs_(COLUMN_MAJOR_IND(j, i, sizeY_)) = flowField_.getRHS().getScalar(i + 1, j + 1);
            }
        }
    }

    void EigenSolver::solve() {
        computeMatrix2D();
        computeRHS2D();

        VectorXd x(dim_);
        BiCGSTAB<SparseMatrix<FLOAT>> solver;

        solver.setMaxIterations(50);
        solver.compute(sparseMatA_);

        x = solver.solve(rhs_);

        std::cout << "#iterations:     " << solver.iterations() << std::endl;
        std::cout << "estimated error: " << solver.error()      << std::endl;

        for (int i = 0; i < sizeX_; i++){
            for (int j = 0; j < sizeY_; j++){
                int row = i*sizeY_ + j;
                flowField_.getPressure().getScalar(i+1, j+1) = x(row);
            }
        }
    }

    inline void EigenSolver::reInitMatrix() {
        printf("Implement me! \n");
    }
} // namespace Solvers::NSEOF

