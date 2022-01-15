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
    SparseMatrix<FLOAT> sparseMatA_;
    VectorXd rhs_;

public:
    void fillConstantsVector();

    void computeMatrixBoundaryLeftOrRight2D(BoundaryType, unsigned int, int);
    void computeMatrixBoundariesBottomAndTop2D();

    void computeMatrix2D();

    EigenSolver(FlowField&, Parameters&);
    ~EigenSolver() override;

    void computeRHS2D();

    void solve() override;
    inline void reInitMatrix() override;
};

} // namespace Solvers::NSEOF

#endif // __SOLVERS_EIGEN_SOLVER_HPP__
