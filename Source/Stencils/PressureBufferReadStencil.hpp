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
 * A boundary stencil that reads the pressure buffer
 */
class PressureBufferReadStencil : public BoundaryStencil<FlowField> {

private:
    std::vector<FLOAT>::iterator pressureBufferLeftIterator_;
    std::vector<FLOAT>::iterator pressureBufferRightIterator_;
    std::vector<FLOAT>::iterator pressureBufferBottomIterator_;
    std::vector<FLOAT>::iterator pressureBufferTopIterator_;
    std::vector<FLOAT>::iterator pressureBufferFrontIterator_;
    std::vector<FLOAT>::iterator pressureBufferBackIterator_;

public:
    explicit PressureBufferReadStencil(const Parameters&);
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

    /**
     * Setters for pressure buffer iterators
     */
    void setPressureBufferLeftIterator   (std::vector<FLOAT>&);
    void setPressureBufferRightIterator  (std::vector<FLOAT>&);
    void setPressureBufferBottomIterator (std::vector<FLOAT>&);
    void setPressureBufferTopIterator    (std::vector<FLOAT>&);
    void setPressureBufferFrontIterator  (std::vector<FLOAT>&);
    void setPressureBufferBackIterator   (std::vector<FLOAT>&);
};

} // namespace NSEOF::Stencils

#endif // __STENCILS_PRESSURE_BUFFER_READ_STENCIL_HPP__
