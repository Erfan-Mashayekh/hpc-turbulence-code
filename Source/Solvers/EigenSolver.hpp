#ifndef __SOLVERS_EIGEN_SOLVER_HPP__
#define __SOLVERS_EIGEN_SOLVER_HPP__

#include "Definitions.hpp"
#include "LinearSolver.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

#include <vector>

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseLU>
#include <Eigen/SparseQR>

// The number of maximum iterations used by the solver (set to -1 to let the solver decide)
#define SOLVER_MAX_NUM_ITERATIONS -1

using namespace Eigen;

namespace NSEOF::Solvers {

struct Constants {
public:
    FLOAT L, R, Bo, T;

    Constants(FLOAT, FLOAT, FLOAT, FLOAT);
};

class EigenSolver : public LinearSolver {
private:
    FlowField& flowField_;
    Parameters& parameters_;

    const int sizeX_;
    const int sizeY_;
    const int dim_;

    std::vector<Constants> constantsVector_;

    MatrixXd matA_;
    VectorXd rhs_;
    VectorXd x_;

    SparseMatrix<FLOAT> sparseMatA_;
    BiCGSTAB<SparseMatrix<FLOAT>> solver_;

public:
    void fillConstantsVector();

    void computeMatrixBoundaryLeftOrRight2D(BoundaryType, unsigned int, int);
    void computeMatrixBoundariesBottomAndTop2D();

    void computeMatrix2D();

    inline void reInitMatrix() override;

    EigenSolver(FlowField&, Parameters&);
    ~EigenSolver() override;

    void computeRHS2D();
    void solve() override;
};

} // namespace Solvers::NSEOF

#endif // __SOLVERS_EIGEN_SOLVER_HPP__
