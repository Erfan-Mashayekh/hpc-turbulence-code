#ifndef __SOLVERS_EIGEN_SOLVER_HPP__
#define __SOLVERS_EIGEN_SOLVER_HPP__

#include "Definitions.hpp"
#include "LinearSolver.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

#include <cmath>
#include <vector>

#ifdef OMP
#include <omp.h>
#endif

// Eigen
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/SparseLU>
#include <Eigen/SparseQR>

// Solver constants
#define SOLVER_ITERATIONS_MAX_NUM 300
#define SOLVER_ITERATIONS_STEP 0.02
#define SOLVER_LOWER_ERROR_THRESHOLD 1e-7

using namespace Eigen;

namespace NSEOF::Solvers {

struct Coefficients {
public:
    FLOAT dxLeft, dxRight, dyBottom, dyTop, dzFront, dzBack;

    Coefficients(FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT);
};

class EigenSolver : public LinearSolver {
private:
    const int cellsX_;
    const int cellsY_;
    const int cellsZ_;
    const int dim_;

    std::vector<Coefficients> coefficientsVector_;

    MatrixXd matA_;
    SparseMatrix<FLOAT> sparseMatA_;
    VectorXd rhs_;
    VectorXd x_;

    BiCGSTAB<SparseMatrix<FLOAT>> solver_;
    int currentNumIterations_ = {};

    void fillCoefficientsVector_();

    void computeStencilRowForFluidCell_(VectorXd&, int, int, int) const;
    void computeStencilRowForObstacleCellWithFluidAround_(int, VectorXd&) const;
    void computeStencilRowForObstacleCell_(VectorXd&) const;

    void computeMatrixOnFluidRegion_(int, int);
    void computeMatrixOnBoundariesLeftAndRight_(unsigned int);
    void computeMatrixOnBoundaryBottomOrTop_(BoundaryType, unsigned int, int);
    void computeMatrixOnBoundaryFrontOrBack_(BoundaryType, unsigned int, int);

    void computeMatrix_();
    void initMatrix_();

    FLOAT getScalarRHS_(int, int, int, int);
    void computeRHS2D_();
    void computeRHS3D_();

    void setPressure2D_();
    void setPressure3D_();

    void updateNumIterationsBasedOnError_();

public:
    EigenSolver(FlowField&, const Parameters&);
    ~EigenSolver() override;

    void solve() override;
    inline void reInitMatrix() override;
};

} // namespace Solvers::NSEOF

#endif // __SOLVERS_EIGEN_SOLVER_HPP__
