#ifndef __GLOBAL_BOUNDARY_FACTORY_HPP__
#define __GLOBAL_BOUNDARY_FACTORY_HPP__

#include "Iterators.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"

#include "Stencils/MovingWallStencils.hpp"
#include "Stencils/PeriodicBoundaryStencils.hpp"
#include "Stencils/NeumannBoundaryStencils.hpp"
#include "Stencils/BFInputStencils.hpp"

#include <string>

namespace NSEOF {

/** Class that returns instances of the global boundary iterator. It also contains the stencils.
 * Right now, it works only with Dirichlet and periodic boundary conditions.
 */
class GlobalBoundaryFactory{
private:
    Stencils::BoundaryStencil<FlowField>* velocityStencils_[6]; //! A stencil for each face
    Stencils::BoundaryStencil<FlowField>* FGHStencils_[6];      //! A stencil for each face
    Stencils::BoundaryStencil<FlowField>* moving_[2];           //! Pointers to the moving wall stencils, if any
    Stencils::BoundaryStencil<FlowField>* periodic_[2];         //! Pointers to the periodic stencils, if any
    Stencils::BoundaryStencil<FlowField>* outflow_[2];          //! Pointers for the outflow conditions
    Stencils::BoundaryStencil<FlowField>* channelInput_[2];     //! For the velocity input
    const Parameters& parameters_;

public:
    GlobalBoundaryFactory(Parameters& parameters);
    ~GlobalBoundaryFactory();

    GlobalBoundaryIterator<FlowField> getGlobalBoundaryFGHIterator(FlowField& flowField);
    GlobalBoundaryIterator<FlowField> getGlobalBoundaryVelocityIterator(FlowField& flowField);
};

} // namespace NSEOF

#endif // __GLOBAL_BOUNDARY_FACTORY_HPP__
