#ifndef __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__
#define __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <vector>
#include <utility>

namespace NSEOF::Stencils {

/**
 * A boundary stencil that reads the velocity buffer
 */
class VelocityBufferReadStencil : public BoundaryStencil<FlowField> {

private:
    std::vector<FLOAT> velocityBufferLeftIterator_;
    std::vector<FLOAT> velocityBufferRightIterator_;
    std::vector<FLOAT> velocityBufferBottomIterator_;
    std::vector<FLOAT> velocityBufferTopIterator_;
    std::vector<FLOAT> velocityBufferFrontIterator_;
    std::vector<FLOAT> velocityBufferBackIterator_;

public:
    explicit VelocityBufferReadStencil(const Parameters&);
    ~VelocityBufferReadStencil() override = default;

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
     * Setters for velocity buffer iterators
     */
    void setVelocityBufferLeftIterator   (std::vector<FLOAT>&);
    void setVelocityBufferRightIterator  (std::vector<FLOAT>&);
    void setVelocityBufferBottomIterator (std::vector<FLOAT>&);
    void setVelocityBufferTopIterator    (std::vector<FLOAT>&);
    void setVelocityBufferFrontIterator  (std::vector<FLOAT>&);
    void setVelocityBufferBackIterator   (std::vector<FLOAT>&);
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VELOCITY_BUFFER_READ_STENCIL_HPP__
