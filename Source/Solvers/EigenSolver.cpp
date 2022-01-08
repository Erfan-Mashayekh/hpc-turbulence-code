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

        MatrixXd matA = MatrixXd::Identity(dim, dim);

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

        for (int i = 0; i < dim; i++) {
            matA(i, i - 1) = 2.0 / (dx_L * (dx_L + dx_R)); // Left
            matA(i, i) = -2.0 / (dx_R * dx_L) - 2.0 / (dx_T * dx_Bo); // Center
            matA(i, i + 1) = 2.0 / (dx_R * (dx_L + dx_R)); // Right
            matA(i, i + sizeX) = 2.0 / (dx_T * (dx_T + dx_Bo)); // Top
            matA(i, i - sizeX) = 2.0 / (dx_Bo * (dx_T + dx_Bo)); // Bottom
        }
        std::cout << matA << std::endl;
        // Boundary Implementations
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
