#ifndef __STENCILS_VELOCITY_BUFFER_FILL_STENCIL_HPP__
#define __STENCILS_VELOCITY_BUFFER_FILL_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <vector>

namespace NSEOF::Stencils {

/**
 * TODO: Write a definition here!
 */
class VelocityBufferFillStencil : public BoundaryStencil<FlowField> {

private:
    std::vector<FLOAT> velocityBufferLeft_;
    std::vector<FLOAT> velocityBufferRight_;
    std::vector<FLOAT> velocityBufferBottom_;
    std::vector<FLOAT> velocityBufferTop_;
    std::vector<FLOAT> velocityBufferFront_;
    std::vector<FLOAT> velocityBufferBack_;

public:
    explicit VelocityBufferFillStencil(const Parameters&);
    ~VelocityBufferFillStencil() override = default;

    /**
     * Returns the velocity buffers filled for sending
     */
    void getVelocityBuffers(std::vector<FLOAT>&, std::vector<FLOAT>&,
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
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_VELOCITY_BUFFER_FILL_STENCIL_HPP__
