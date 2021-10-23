#ifndef __SOLVERS_LINEAR_SOLVER_HPP__
#define __SOLVERS_LINEAR_SOLVER_HPP__

#include "Definitions.hpp"
#include "Parameters.hpp"
#include "FlowField.hpp"

namespace NSEOF {
namespace Solvers {

// Abstract class for linear solvers for the pressure
class LinearSolver {
protected:
    FlowField& flowField_;
    const Parameters& parameters_;

public:
    LinearSolver(FlowField& flowField, const Parameters& parameters);
    virtual ~LinearSolver() = default;

    virtual void solve() = 0;
    virtual inline void reInitMatrix() {}
};

} // namespace Solvers
} // namespace NSEOF

#endif // __SOLVERS_LINEAR_SOLVER_HPP__
