#ifndef __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__
#define __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <vector>
#include <utility>

namespace NSEOF::Stencils {

/**
 * TODO: Write a definition here!
 */
class PressureBufferReadStencil : public BoundaryStencil<FlowField> {

private:
    std::vector<FLOAT> pressureBufferLeft_;
    std::vector<FLOAT> pressureBufferRight_;
    std::vector<FLOAT> pressureBufferBottom_;
    std::vector<FLOAT> pressureBufferTop_;
    std::vector<FLOAT> pressureBufferFront_;
    std::vector<FLOAT> pressureBufferBack_;

public:
    explicit PressureBufferReadStencil(const Parameters&);
    ~PressureBufferReadStencil() override;

    /**
     * Sets the pressure buffers received
     */
    void setPressureBuffers(std::vector<FLOAT>&, std::vector<FLOAT>&,
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

#endif // __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__
