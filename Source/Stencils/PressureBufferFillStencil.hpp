#ifndef __STENCILS_PRESSURE_BUFFER_FILL_STENCIL_HPP__
#define __STENCILS_PRESSURE_BUFFER_FILL_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <vector>

namespace NSEOF::Stencils {

/**
 * TODO: Write a definition here!
 */
class PressureBufferFillStencil : public BoundaryStencil<FlowField> {

private:
    std::vector<FLOAT> pressureBufferLeft_;
    std::vector<FLOAT> pressureBufferRight_;
    std::vector<FLOAT> pressureBufferBottom_;
    std::vector<FLOAT> pressureBufferTop_;
    std::vector<FLOAT> pressureBufferFront_;
    std::vector<FLOAT> pressureBufferBack_;

public:
    explicit PressureBufferFillStencil(const Parameters&);
    ~PressureBufferFillStencil() override;

    /**
     * Returns the pressure buffers filled for sending
     */
    void getPressureBuffers(std::vector<FLOAT>&, std::vector<FLOAT>&,
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

#endif // __STENCILS_PRESSURE_BUFFER_FILL_STENCIL_HPP__
