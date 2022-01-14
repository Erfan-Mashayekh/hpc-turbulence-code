#ifndef __SOLVERS_EIGEN_SOLVER_HPP__
#define __SOLVERS_EIGEN_SOLVER_HPP__

#include "LinearSolver.hpp"
#include "DataStructures.hpp"

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseLU>
#include <Eigen/SparseQR>

using namespace Eigen;

namespace NSEOF::Solvers {

class EigenSolver : public LinearSolver {
private:
    FlowField& flowField_;
    Parameters& parameters_;

    const int sizeX_;
    const int sizeY_;
    const int dim_;

    MatrixXd matA_;
    SparseMatrix<FLOAT> sparseMatA_;
    VectorXd rhs_;

public:
    EigenSolver(FlowField&, Parameters&);
    virtual ~EigenSolver() = default;

    void solve() override;
    inline void reInitMatrix() override;

    void computeMatrix2D();
    void computeRHS2D();
};

} // namespace Solvers::NSEOF

#endif // __SOLVERS_EIGEN_SOLVER_HPP__
