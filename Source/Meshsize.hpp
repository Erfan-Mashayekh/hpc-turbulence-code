#ifndef __MESHSIZE_HPP__
#define __MESHSIZE_HPP__

#include "Definitions.hpp"

#include <cmath>

namespace NSEOF {

// Forward declaration of Parameters
class Parameters;

enum MeshsizeType {
    Uniform = 0,
    TanhStretching = 1
};

class Meshsize {
public:
    Meshsize() = default;
    virtual ~Meshsize() = default;

    // Returns the meshsize of cell i, j or i, j, k, respectively.
    virtual FLOAT getDx(int i, int j) const = 0;
    virtual FLOAT getDy(int i, int j) const = 0;

    virtual FLOAT getDx(int i, int j, int k) const = 0;
    virtual FLOAT getDy(int i, int j, int k) const = 0;
    virtual FLOAT getDz(int i, int j, int k) const = 0;

    // Returns the global geometric position in x-, y-, z-direction
    // of the lower/left/front corner of the local cell at (i, j, k).
    virtual FLOAT getPosX(int i, int j, int k) const = 0;
    virtual FLOAT getPosY(int i, int j, int k) const = 0;
    virtual FLOAT getPosZ(int i, int j, int k) const = 0;

    virtual FLOAT getPosX(int i, int j) const = 0;
    virtual FLOAT getPosY(int i, int j) const = 0;

    // Returns the min. meshsize used in this simulation
    // -> required for adaptive time stepping.
    virtual FLOAT getDxMin() const = 0;
    virtual FLOAT getDyMin() const = 0;
    virtual FLOAT getDzMin() const = 0;
};

/** Implements a uniform, equidistant grid spacing */
class UniformMeshsize : public Meshsize {
private:
    const FLOAT dx_;
    const FLOAT dy_;
    const FLOAT dz_;
    const int firstCornerX_;
    const int firstCornerY_;
    const int firstCornerZ_;

public:
    UniformMeshsize(const Parameters& parameters);
    virtual ~UniformMeshsize() override = default;

    virtual FLOAT getDx([[maybe_unused]] int i, [[maybe_unused]] int j) const override { return dx_; }
    virtual FLOAT getDy([[maybe_unused]] int i, [[maybe_unused]] int j) const override { return dy_; }

    virtual FLOAT getDx([[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) const override { return dx_; }
    virtual FLOAT getDy([[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) const override { return dy_; }
    virtual FLOAT getDz([[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) const override { return dz_; }

    virtual FLOAT getPosX([[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) const override { return dx_ * (firstCornerX_ - 2 + i); }
    virtual FLOAT getPosY([[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) const override { return dy_ * (firstCornerY_ - 2 + j); }
    virtual FLOAT getPosZ([[maybe_unused]] int i, [[maybe_unused]] int j, [[maybe_unused]] int k) const override { return dz_ * (firstCornerZ_ - 2 + k); }

    virtual FLOAT getPosX(int i, int j) const override { return getPosX(i, j, 0); }
    virtual FLOAT getPosY(int i, int j) const override { return getPosY(i, j, 0); }

    virtual FLOAT getDxMin() const override { return dx_; }
    virtual FLOAT getDyMin() const override { return dy_; }
    virtual FLOAT getDzMin() const override { return dz_; }
};

/** Implements a stretched mesh for e.g. channel flow. For each dimension, a stretching of the mesh can be introduced towards
 *  the outer boundaries, i.e. if stretchX is true (in constructor), then the mesh will be finer close to the left and right
 *  boundary. The stretching is based on a formular involving tanh-functions, as e.g. used in the dissertation by Tobias Neckel,
 *  Chair of Scientific Computing in Computer Science (TUM SCCS).
 *  For non-stretched meshes, the UniformMeshsize implementation is used to create a uniform mesh.
 */
class TanhMeshStretching: public Meshsize {
private:
    const UniformMeshsize uniformMeshsize_;
    const FLOAT lengthX_;
    const FLOAT lengthY_;
    const FLOAT lengthZ_;
    const int sizeX_;
    const int sizeY_;
    const int sizeZ_;
    const int firstCornerX_;
    const int firstCornerY_;
    const int firstCornerZ_;
    const bool stretchX_;
    const bool stretchY_;
    const bool stretchZ_;
    const FLOAT deltaS_;
    const FLOAT tanhDeltaS_;
    const FLOAT dxMin_;
    const FLOAT dyMin_;
    const FLOAT dzMin_;

    // Computes the coordinate of the lower/left/front corner of the 1D-cell at index i w.r.t. having "size" cells along
    // an interval of length "length". We refer to local indexing, so "firstCorner" denotes the first non-ghost cell index
    // of this process. We use a stretched mesh for all nodes inside the comput. bounding box, and a regular mesh outside this box,
    // using the meshsize of the next inner cell.
    FLOAT computeCoordinate(int i, int firstCorner, int size, FLOAT length, FLOAT dxMin) const {
        const int index = i - 2 + firstCorner;
        if (index < 0) {
            // Equidistant mesh on lower/left part
            return dxMin * index;
        } else if (index > size - 1) {
            // Equidistant mesh on upper/right part
            return length + dxMin * (index - size);
        } else {
            // Stretched mesh on lower half of channel -> we check if we are in lower 50% and then use stretching for 2.0 * p
            FLOAT p = ((FLOAT) index) / size;
            if (p < 0.5) {
                return 0.5 * length * (1.0 + tanh(deltaS_ * (2.0 * p - 1.0)) / tanhDeltaS_);
            } else {
                // Stretched mesh on upper half of channel -> we mirror the stretching
                p = ((FLOAT) size - index) / size;
                return length - 0.5 * length * (1.0 + tanh(deltaS_ * (2.0 * p - 1.0)) / tanhDeltaS_);
            }
        }
    }

    // Returns the meshsize based on vertex coordinates that span the respective 1D-cell
    FLOAT getMeshsize(int i, int firstCorner, int size, FLOAT length, FLOAT dxMin) const {
        const FLOAT pos0 = computeCoordinate(i, firstCorner, size, length, dxMin);
        const FLOAT pos1 = computeCoordinate(i + 1, firstCorner, size, length, dxMin);
#ifndef NDEBUG
        if (pos1 - pos0 < 1.0e-12) {
            HANDLE_ERROR(1, "Error TanhMeshStretching::getMeshsize(): dx < 1.0e-12!");
        }
#endif
        return pos1 - pos0;
    }

public:
    TanhMeshStretching(const Parameters& parameters, bool stretchX, bool stretchY, bool stretchZ);
    virtual ~TanhMeshStretching() = default;

    virtual FLOAT getDx(int i, int j) const override {
        if (stretchX_) {
            return getMeshsize(i, firstCornerX_, sizeX_, lengthX_, dxMin_);
        } else {
            return uniformMeshsize_.getDx(i, j);
        }
    }

    virtual FLOAT getDy(int i, int j) const override {
        if (stretchY_) {
            return getMeshsize(j, firstCornerY_, sizeY_, lengthY_, dyMin_);
        } else {
            return uniformMeshsize_.getDy(i, j);
        }
    }

    virtual FLOAT getDx(int i, int j, [[maybe_unused]] int k) const override {
        return getDx(i, j);
    }

    virtual FLOAT getDy(int i, int j, [[maybe_unused]] int k) const override {
        return getDy(i, j);
    }

    virtual FLOAT getDz(int i, int j, int k) const override {
        if (stretchZ_) {
            return getMeshsize(k, firstCornerZ_, sizeZ_, lengthZ_, dzMin_);
        } else {
            return uniformMeshsize_.getDz(i, j, k);
        }
    }

    virtual FLOAT getPosX(int i, int j, int k) const override {
        if (stretchX_) {
            return computeCoordinate(i, firstCornerX_, sizeX_, lengthX_, dxMin_);
        } else {
            return uniformMeshsize_.getPosX(i, j, k);
        }
    }

    virtual FLOAT getPosY(int i, int j, int k) const override {
        if (stretchY_) {
            return computeCoordinate(j, firstCornerY_, sizeY_, lengthY_, dyMin_);
        } else {
            return uniformMeshsize_.getPosY(i, j, k);
        }
    }

    virtual FLOAT getPosZ(int i, int j, int k) const override {
        if (stretchZ_) {
            return computeCoordinate(k, firstCornerZ_, sizeZ_, lengthZ_, dzMin_);
        } else {
            return uniformMeshsize_.getPosZ(i, j, k);
        }
    }

    virtual FLOAT getPosX(int i, int j) const override { return getPosX(i, j, 0); }
    virtual FLOAT getPosY(int i, int j) const override { return getPosY(i, j, 0); }

    virtual FLOAT getDxMin() const override { return dxMin_; }
    virtual FLOAT getDyMin() const override { return dyMin_; }
    virtual FLOAT getDzMin() const override { return dzMin_; }
};

} // namespace NSEOF

#endif // __MESHSIZE_HPP__
