#ifndef __MESHSIZE_FACTORY_HPP__
#define __MESHSIZE_FACTORY_HPP__

#include "Parameters.hpp"
#include "Definitions.hpp"

namespace NSEOF {

/** Initialises the meshsize in the Parameters. Must be called after configuring (Configuration and PetscParallelConfiguration).
 *  We therefore make use of the singleton/factory pattern.
 */
class MeshsizeFactory {
private:
    MeshsizeFactory() = default;

public:
    ~MeshsizeFactory() = default;

    static MeshsizeFactory& getInstance();

    void initMeshsize(Parameters& parameters);
};

} // namespace NSEOF

#endif // __MESHSIZE_FACTORY_HPP__
