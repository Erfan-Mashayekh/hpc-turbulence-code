#include "EigenSolver.hpp"

namespace NSEOF::Solvers {

EigenSolver::EigenSolver(FlowField& flowField, Parameters& parameters)
    : LinearSolver(flowField, parameters)
    , flowField_(flowField)
    , parameters_(parameters)
    , sizeX_(parameters.parallel.localSize[0] + 2)
    , sizeY_(parameters_.parallel.localSize[1] + 2)
    , dim_(sizeX_ * sizeY_)
    , matA_(MatrixXd::Zero(dim_, dim_))
    , rhs_(VectorXd::Zero(dim_)) {}

    void EigenSolver::computeMatrix2D() {
        FLOAT dx_0, dx_M1, dx_P1, dy_0, dy_M1, dy_P1;
        FLOAT dx_L, dx_R, dx_Bo, dx_T;

        for (int i = 0; i < sizeX_; i++) {
            for (int j = 0; j < sizeY_; j++) {
                dx_0 = parameters_.meshsize->getDx(i, j);
                dx_M1 = parameters_.meshsize->getDx(i - 1, j);
                dx_P1 = parameters_.meshsize->getDx(i + 1, j);
                dy_0 = parameters_.meshsize->getDy(i, j);
                dy_M1 = parameters_.meshsize->getDy(i, j - 1);
                dy_P1 = parameters_.meshsize->getDy(i, j + 1);

                dx_L = 0.5 * (dx_0 + dx_M1);
                dx_R = 0.5 * (dx_0 + dx_P1);
                dx_Bo = 0.5 * (dy_0 + dy_M1);
                dx_T = 0.5 * (dy_0 + dy_P1);
            }
        }


        //dx_0 = parameters_.meshsize->getDx(1, 1);
        //dx_M1 = parameters_.meshsize->getDx(1, 1);
        //dx_P1 = parameters_.meshsize->getDx(1, 1);
        //dy_0 = parameters_.meshsize->getDy(1, 1);
        //dy_M1 = parameters_.meshsize->getDy(1, 1);
        //dy_P1 = parameters_.meshsize->getDy(1, 1);

        //dx_L = 0.5 * (dx_0 + dx_M1);
        //dx_R = 0.5 * (dx_0 + dx_P1);
        //dx_Bo = 0.5 * (dy_0 + dy_M1);
        //dx_T = 0.5 * (dy_0 + dy_P1);


        for (int row = sizeY_+1; row < dim_ - (sizeY_+1); row++) {
            matA_(row, row - sizeY_) = 2.0 / (dx_L * (dx_L + dx_R)); // Left
            matA_(row, row) = -2.0 / (dx_R * dx_L) - 2.0 / (dx_T * dx_Bo); // Center
            matA_(row, row + sizeY_) = 2.0 / (dx_R * (dx_L + dx_R)); // Right
            matA_(row, row + 1) = 2.0 / (dx_T * (dx_T + dx_Bo)); // Top
            matA_(row, row - 1) = 2.0 / (dx_Bo * (dx_T + dx_Bo)); // Bottom
        }

    	// Boundary Implementations
        // Left wall
        // VectorXd boundaryDirichlet(sizeY_ * 2);
        // VectorXd boundaryNeumann(sizeY_ * 2);

        // boundaryDirichlet.segment<(sizeY_ - 2)>(1) = VectorXd::Ones(sizeY_ - 2);

        for (int j = 1; j < sizeY_-1; j++) {
            int i = 0;
            int row = i*sizeY_ + j;
            if (parameters_.walls.typeLeft == DIRICHLET) { // If Dirichlet velocity boundary conditions
                // Therefore, Neumann in the pressure
                matA_(row, row) = 1.0; // Center
                matA_(row, row + sizeY_) = -1.0; // Right
            } else if (parameters_.walls.typeLeft == NEUMANN) { // Neumann velocity boundary conditions
                matA_(row, row) = 0.5; // Center
                matA_(row, row + sizeY_) = 0.5; // Right
            }
        }

        // Right wall
        for (int j = 1; j < sizeY_-1; j++) {
            int i = sizeX_-1;
            int row = i*sizeY_ + j;
            if (parameters_.walls.typeRight == DIRICHLET) { // If Dirichlet velocity boundary conditions
                // Therefore, Neumann in the pressure
                matA_(row, row) = 1.0; // Center
                matA_(row, row - sizeY_) = -1.0; // Left
            } else if (parameters_.walls.typeRight == NEUMANN) { // Neumann velocity boundary conditions
                matA_(row, row) = 0.5; // Center
                matA_(row, row - sizeY_) = 0.5; // Left
            }
        }

        // Bottom wall
        for (int i = 1; i < sizeX_-1; i++) {
            int j = 0;
            int row = i*sizeY_ + j;
            if (parameters_.walls.typeBottom == DIRICHLET) { // If Dirichlet velocity boundary conditions
                // Therefore, Neumann in the pressure
                matA_(row, row) = 1.0; // Center
                matA_(row, row + 1) = -1.0; // Top
                matA_(row, row - sizeY_) = 0.0; // Left
                matA_(row, row + sizeY_) = 0.0; // Right
                matA_(row, row - 1) = 0.0; // Bottom
            } else if (parameters_.walls.typeBottom == NEUMANN) { // Neumann velocity boundary conditions
                matA_(row, row) = 0.5; // Center
                matA_(row, row + 1) = 0.5; // Top
                matA_(row, row - sizeY_) = 0.0; // Left
                matA_(row, row + sizeY_) = 0.0; // Right
                matA_(row, row - 1) = 0.0; // Bottom
            }
        }

        // Top wall
        for (int i = 1; i < sizeX_-1; i++) {
            int j = sizeY_-1;
            int row = i*sizeY_ + j;
            if (parameters_.walls.typeTop == DIRICHLET) { // If Dirichlet velocity boundary conditions
                // Therefore, Neumann in the pressure
                matA_(row, row) = 1.0; // Center
                matA_(row, row - 1) = -1.0; // Bottom
                matA_(row, row - sizeY_) = 0.0; // Left
                matA_(row, row + sizeY_) = 0.0; // Right
                matA_(row, row + 1) = 0.0; // Top
            } else if (parameters_.walls.typeTop == NEUMANN) { // Neumann velocity boundary conditions
                matA_(row, row) = 0.5; // Center
                matA_(row, row - 1) = 0.5; // Bottom
                matA_(row, row - sizeY_) = 0.0; // Left
                matA_(row, row + sizeY_) = 0.0; // Right
                matA_(row, row + 1) = 0.0; // Top
            }
        }

        sparseMatA_ = matA_.sparseView();
        
        // std::cout << matA_ << std::endl;
        // std::cout << sparseMatA_ << std::endl;

    }

    void EigenSolver::computeRHS2D() {
        for (int i = 1; i < sizeX_ - 1; i++){
            for (int j = 1; j < sizeY_ - 1; j++){
                int row = i*sizeY_ + j;
                rhs_(row) = flowField_.getRHS().getScalar(i+1, j+1);
            }
        }
        //std::cout << rhs << std::endl;

    }

    void EigenSolver::solve() {
        computeMatrix2D();
        computeRHS2D();

        VectorXd x(dim_);

        //BiCGSTAB<SparseMatrix<FLOAT>> solver;
        //solver.compute(sparseMatA_);
        //x = solver.solve(rhs_);

        //std::cout << "#iterations:     " << solver.iterations() << std::endl;
        //std::cout << "estimated error: " << solver.error()      << std::endl;

        SparseLU<SparseMatrix<FLOAT>, COLAMDOrdering<int> >   solver;
        // Compute the ordering permutation vector from the structural pattern of A
        solver.analyzePattern(sparseMatA_); 
        // Compute the numerical factorization 
        solver.factorize(sparseMatA_); 
        //Use the factors to solve the linear system 
        x = solver.solve(rhs_);

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

