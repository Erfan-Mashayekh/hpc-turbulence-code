#ifndef __SOLVERS_SOR_SOLVER_HPP__
#define __SOLVERS_SOR_SOLVER_HPP__

#include "LinearSolver.hpp"

namespace NSEOF {
namespace Solvers {

class SORSolver : public LinearSolver {
public:
    SORSolver(FlowField& flowField, const Parameters& parameters);
    ~SORSolver() override = default;

    void solve() override;
};

} // namespace Solvers
} // namespace NSEOF

#endif // __SOLVERS_SOR_SOLVER_HPP__
