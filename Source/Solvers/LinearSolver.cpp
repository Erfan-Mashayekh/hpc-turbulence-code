#include "LinearSolver.hpp"

namespace NSEOF {
namespace Solvers {

LinearSolver::LinearSolver(FlowField& flowField, const Parameters& parameters)
    : flowField_(flowField)
    , parameters_(parameters) {}

} // namespace Solvers
} // namespace NSEOF
