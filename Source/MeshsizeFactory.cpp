#include "MeshsizeFactory.hpp"

namespace NSEOF {

MeshsizeFactory& MeshsizeFactory::getInstance(){
    static MeshsizeFactory singleton;
    return singleton;
}

void MeshsizeFactory::initMeshsize(Parameters& parameters) {
    // Initialise meshsize
    switch (parameters.geometry.meshsizeType) {
    // Uniform meshsize
    case Uniform:
        parameters.meshsize = new UniformMeshsize(parameters);
        break;
    // Tanh-stretched mesh
    case TanhStretching:
        parameters.meshsize = new TanhMeshStretching(parameters,
            (bool) parameters.geometry.stretchX,
            (bool) parameters.geometry.stretchY,
            (bool) parameters.geometry.stretchZ);
        break;
    default:
        HANDLE_ERROR(1, "Unknown meshsize type!");
        break;
    }

    if (parameters.meshsize == NULL) {
        HANDLE_ERROR(1, "parameters.meshsize == NULL!");
    }
}

} // namespace NSEOF
