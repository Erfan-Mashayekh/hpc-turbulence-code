#ifndef __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__
#define __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <vector>

namespace NSEOF::Stencils {

/**
 * TODO: Write a definition here!
 */
class VelocityBufferReadStencil : public BoundaryStencil<FlowField> {

private:
    std::vector<FLOAT>::iterator velocityBufferLeftIterator_;
    std::vector<FLOAT>::iterator velocityBufferRightIterator_;
    std::vector<FLOAT>::iterator velocityBufferBottomIterator_;
    std::vector<FLOAT>::iterator velocityBufferTopIterator_;
    std::vector<FLOAT>::iterator velocityBufferFrontIterator_;
    std::vector<FLOAT>::iterator velocityBufferBackIterator_;

public:
    explicit VelocityBufferReadStencil(const Parameters&);
    ~VelocityBufferReadStencil() override = default;

    /**
     * Sets the velocity buffers received
     */
    void setVelocityBuffers(std::vector<FLOAT>&, std::vector<FLOAT>&,
                            std::vector<FLOAT>&, std::vector<FLOAT>&,
                            std::vector<FLOAT>&, std::vector<FLOAT>&);

    /**
     * Functions for 3D
     */
    void applyLeftWall   (FlowField&, int, int, int) override;
    void applyRightWall  (FlowField&, int, int, int) override;
    void applyBottomWall (FlowField&, int, int, int) override;
    void applyTopWall    (FlowField&, int, int, int) override;
    void applyFrontWall  (FlowField&, int, int, int) override;
    void applyBackWall   (FlowField&, int, int, int) override;

    /**
     * Functions for 2D
     */
    void applyLeftWall   (FlowField&, int, int) override;
    void applyRightWall  (FlowField&, int, int) override;
    void applyBottomWall (FlowField&, int, int) override;
    void applyTopWall    (FlowField&, int, int) override;

    /**
     * Setters for the pressure buffer iterators
     */
    void setVelocityBufferLeftIterator(std::vector<FLOAT>&);
    void setVelocityBufferRightIterator(std::vector<FLOAT>&);
    void setVelocityBufferBottomIterator(std::vector<FLOAT>&);
    void setVelocityBufferTopIterator(std::vector<FLOAT>&);
    void setVelocityBufferFrontIterator(std::vector<FLOAT>&);
    void setVelocityBufferBackIterator(std::vector<FLOAT>&);
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__
