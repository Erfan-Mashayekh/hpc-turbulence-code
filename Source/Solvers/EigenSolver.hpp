#ifndef __SOLVERS_EIGEN_SOLVER_HPP__
#define __SOLVERS_EIGEN_SOLVER_HPP__

#include "LinearSolver.hpp"
#include "DataStructures.hpp"
#include <Eigen/Core>
#include <Eigen/Sparse>
#include<Eigen/IterativeLinearSolvers>

namespace NSEOF::Solvers {

class EigenSolver : public LinearSolver {
protected:
    FlowField& flowField_;
    Parameters& parameters_;

public:
    EigenSolver(FlowField&, Parameters&);
    virtual ~EigenSolver() = default;

    void solve() override;
    inline void reInitMatrix() override;

    Eigen::SparseMatrix<FLOAT, 0, int> computeMatrix2D();
    Eigen::VectorXd computeRHS2D();
};

} // namespace Solvers::NSEOF

#endif // __SOLVERS_EIGEN_SOLVER_HPP__
