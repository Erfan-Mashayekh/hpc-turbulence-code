#ifndef __SOLVERS_EIGEN_SOLVER_HPP__
#define __SOLVERS_EIGEN_SOLVER_HPP__

#include "Definitions.hpp"
#include "LinearSolver.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

#include <cmath>
#include <vector>

// Eigen
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
    FLOAT dxLeft, dxRight, dyBottom, dyTop, dzFront, dzBack;

    Constants(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT);
};

class EigenSolver : public LinearSolver {
private:
    const int cellsX_;
    const int cellsY_;
    const int cellsZ_;
    const int dim_;

    std::vector<Constants> constantsVector_;

    MatrixXd matA_;
    VectorXd rhs_;
    VectorXd x_;

    SparseMatrix<FLOAT> sparseMatA_;
    BiCGSTAB<SparseMatrix<FLOAT>> solver_;

    void fillConstantsVector_();

    int getObstacle_(int, int, int, int, int, int) const;
    int getSumObstacles_() const;

    void computeStencilRowForFluidCell_(int, VectorXd&, int, int, int) const;
    void computeStencilRowForObstacleCellWithFluidAround_(int, int, VectorXd&) const;
    void computeStencilRowForObstacleCell_(int, VectorXd&) const;

    void computeMatrixBoundaryLeftOrRight2D_(BoundaryType, unsigned int, int);
    void computeMatrixBoundariesBottomAndTop2D_();

    void computeMatrix2D_();
    void computeRHS_();

    void initMatrix_();

public:
    EigenSolver(FlowField&, const Parameters&);
    ~EigenSolver() override;

    void solve() override;
    inline void reInitMatrix() override;
};

} // namespace Solvers::NSEOF

#endif // __SOLVERS_EIGEN_SOLVER_HPP__
