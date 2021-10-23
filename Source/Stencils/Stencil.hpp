#ifndef __STENCILS_STENCIL_HPP__
#define __STENCILS_STENCIL_HPP__

#include "Parameters.hpp"

namespace NSEOF {
namespace Stencils {

/** Stencil class
 *
 * Abstract class for the definition of stencils and operations on the grids
 */
template <class FlowFieldType>
class FieldStencil {
protected:
    const Parameters& parameters_; //! Reference to the parameters

public:
    FieldStencil(const Parameters & parameters)
        : parameters_(parameters) {}

    virtual ~FieldStencil() = default;

    /** Performs the operation in 2D in a given position
     * @param flowField Flow field data
     * @param parameters Parameters of the problem
     * @param i Position in the x direction
     * @param j Position in the y direction
     */
    virtual void apply(FlowFieldType& flowField, int i, int j) = 0;

    /** Performs the operation in 3D in a given position
     * @param flowField Flow field data
     * @param parameters Parameters of the problem
     * @param i Position in the x direction
     * @param j Position in the y direction
     * @param k Position in the z direction
     */
    virtual void apply(FlowFieldType& flowField, int i, int j, int k) = 0;
};

/** Interface for operations on the global (or parallel) boundary
 */
template <class FlowFieldType>
class BoundaryStencil {
protected:
    const Parameters& parameters_;

public:
    BoundaryStencil(const Parameters & parameters)
        : parameters_(parameters) {}

    virtual ~BoundaryStencil() = default;

    /** Represents an operation in the left wall of a 2D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     */
    virtual void applyLeftWall(FlowFieldType& flowField, int i, int j) = 0;

    /** Represents an operation in the right wall of a 2D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     */
    virtual void applyRightWall(FlowFieldType& flowField, int i, int j) = 0;

    /** Represents an operation in the bottom wall of a 2D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     */
    virtual void applyBottomWall(FlowFieldType& flowField, int i, int j) = 0;

    /** Represents an operation in the top wall of a 2D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     */
    virtual void applyTopWall(FlowFieldType& flowField, int i, int j) = 0;

    /** Represents an operation in the left wall of a 3D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     * @param k Index in the z direction
     */
    virtual void applyLeftWall(FlowFieldType& flowField, int i, int j, int k) = 0;

    /** Represents an operation in the right wall of a 3D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     * @param k Index in the z direction
     */
    virtual void applyRightWall(FlowFieldType& flowField, int i, int j, int k) = 0;

    /** Represents an operation in the bottom wall of a 3D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     * @param k Index in the z direction
     */
    virtual void applyBottomWall(FlowFieldType& flowField, int i, int j, int k) = 0;

    /** Represents an operation in the top wall of a 3D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     * @param k Index in the z direction
     */
    virtual void applyTopWall(FlowFieldType& flowField, int i, int j, int k) = 0;

    /** Represents an operation in the front wall of a 3D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     * @param k Index in the z direction
     */
    virtual void applyFrontWall(FlowFieldType& flowField, int i, int j, int k) = 0;

    /** Represents an operation in the back wall of a 3D domain.
     *
     * @param flowField State of the flow field
     * @param i Index in the x direction
     * @param j Index in the y direction
     * @param k Index in the z direction
     */
    virtual void applyBackWall(FlowFieldType& flowField, int i, int j, int k) = 0;
};

} // namespace Stencils
} // namespace NSEOF

#endif // __STENCILS_STENCIL_HPP__
