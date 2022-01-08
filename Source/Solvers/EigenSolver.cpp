#include "EigenSolver.hpp"

#include <Eigen/Core>

using namespace Eigen;

namespace NSEOF::Solvers {

EigenSolver::EigenSolver(FlowField& flowField, Parameters& parameters)
    : flowField_(flowField)
    , parameters_(parameters)
    , LinearSolver(flowField, parameters) {}

    void EigenSolver::computeMatrix2D() {
        const int sizeX = parameters_.parallel.localSize[0] + 2;
        const int sizeY = parameters_.parallel.localSize[1] + 2;
        const int dim = sizeX * sizeY;

        MatrixXd matA = MatrixXd::Zero(dim, dim);

        FLOAT dx_0 ;
        FLOAT dx_M1;
        FLOAT dx_P1;
        FLOAT dy_0 ;
        FLOAT dy_M1;
        FLOAT dy_P1;
        
        FLOAT dx_L ;
        FLOAT dx_R ;
        FLOAT dx_Bo;
        FLOAT dx_T ;

        for (int i = 0; i < sizeX; i++) {
            for (int j = 0; j < sizeY; j++) {
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

        for (int j = sizeY+1; j < dim - (sizeY+1); j++) {
            matA(j, j - sizeY) = 2.0 / (dx_L * (dx_L + dx_R)); // Left
            matA(j, j) = -2.0 / (dx_R * dx_L) - 2.0 / (dx_T * dx_Bo); // Center
            matA(j, j + sizeY) = 2.0 / (dx_R * (dx_L + dx_R)); // Right
            matA(j, j + 1) = 2.0 / (dx_T * (dx_T + dx_Bo)); // Top
            matA(j, j - 1) = 2.0 / (dx_Bo * (dx_T + dx_Bo)); // Bottom
        }

        // Boundary Implementations
        // Left wall
        for (int j = 1; j < sizeY-1; j++) {
            int i = 0;
            int row = i*sizeY + j;
            if (parameters_.walls.typeLeft == DIRICHLET) { // If Dirichlet velocity boundary conditions
                // Therefore, Neumann in the pressure
                matA(row, row) = 1.0; // Center
                matA(row, row + sizeY) = -1.0; // Right
            } else if (parameters_.walls.typeLeft == NEUMANN) { // Neumann velocity boundary conditions
                matA(row, row) = 0.5; // Center
                matA(row, row + sizeY) = 0.5; // Right
            }
        }

        // Right wall
        for (int j = 1; j < sizeY-1; j++) {
            int i = sizeX-1;
            int row = i*sizeY + j;
            if (parameters_.walls.typeRight == DIRICHLET) { // If Dirichlet velocity boundary conditions
                // Therefore, Neumann in the pressure
                matA(row, row) = 1.0; // Center
                matA(row, row - sizeY) = -1.0; // Left
            } else if (parameters_.walls.typeRight == NEUMANN) { // Neumann velocity boundary conditions
                matA(row, row) = 0.5; // Center
                matA(row, row - sizeY) = 0.5; // Left
            }
        }

        // Bottom wall
        for (int i = 1; i < sizeX-1; i++) {
            int j = 0;
            int row = i*sizeY + j;
            if (parameters_.walls.typeBottom == DIRICHLET) { // If Dirichlet velocity boundary conditions
                // Therefore, Neumann in the pressure
                matA(row, row) = 1.0; // Center
                matA(row, row + 1) = -1.0; // Top
                matA(row, row - sizeY) = 0.0; // Left
                matA(row, row + sizeY) = 0.0; // Right
                matA(row, row - 1) = 0.0; // Bottom
            } else if (parameters_.walls.typeBottom == NEUMANN) { // Neumann velocity boundary conditions
                matA(row, row) = 0.5; // Center
                matA(row, row + 1) = 0.5; // Top
                matA(row, row - sizeY) = 0.0; // Left
                matA(row, row + sizeY) = 0.0; // Right
                matA(row, row - 1) = 0.0; // Bottom
            }
        }

        // Top wall
        for (int i = 1; i < sizeX-1; i++) {
            int j = sizeY-1;
            int row = i*sizeY + j;
            if (parameters_.walls.typeTop == DIRICHLET) { // If Dirichlet velocity boundary conditions
                // Therefore, Neumann in the pressure
                matA(row, row) = 1.0; // Center
                matA(row, row - 1) = -1.0; // Bottom
                matA(row, row - sizeY) = 0.0; // Left
                matA(row, row + sizeY) = 0.0; // Right
                matA(row, row + 1) = 0.0; // Top
            } else if (parameters_.walls.typeTop == NEUMANN) { // Neumann velocity boundary conditions
                matA(row, row) = 0.5; // Center
                matA(row, row - 1) = 0.5; // Bottom
                matA(row, row - sizeY) = 0.0; // Left
                matA(row, row + sizeY) = 0.0; // Right
                matA(row, row + 1) = 0.0; // Top
            }
        }
        //std::cout << matA << std::endl;
    }

    void EigenSolver::computeRHS2D() {
        const int dim = parameters_.parallel.localSize[0] * parameters_.parallel.localSize[1];
        VectorXd rhs = VectorXd::Zero(dim);
    }

    void EigenSolver::solve() {
        computeMatrix2D();
    }

    inline void EigenSolver::reInitMatrix() {
        printf("Implement me! \n");
    }
} // namespace Solvers::NSEOF
