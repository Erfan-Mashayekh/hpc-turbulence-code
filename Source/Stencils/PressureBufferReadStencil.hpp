#ifndef __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__
#define __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__

#include "Stencil.hpp"
#include "FlowField.hpp"
#include "Parameters.hpp"
#include "Definitions.hpp"

#include <vector>

namespace NSEOF::Stencils {

/**
 * TODO: Write a definition here!
 */
class PressureBufferReadStencil : public BoundaryStencil<FlowField> {

private:
    std::vector<FLOAT> pressuresLeft_;
    std::vector<FLOAT> pressuresRight_;
    std::vector<FLOAT> pressuresBottom_;
    std::vector<FLOAT> pressuresTop_;
    std::vector<FLOAT> pressuresFront_;
    std::vector<FLOAT> pressuresBack_;

public:
    PressureBufferReadStencil(const Parameters&,
                              std::vector<FLOAT>&, std::vector<FLOAT>&,
                              std::vector<FLOAT>&, std::vector<FLOAT>&,
                              std::vector<FLOAT>&, std::vector<FLOAT>&);
    ~PressureBufferReadStencil() override = default;

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
