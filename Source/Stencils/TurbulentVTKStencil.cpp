#include "TurbulentVTKStencil.hpp"
#include "StencilFunctions.hpp"
#include "Definitions.hpp"

namespace NSEOF::Stencils {

    TurbulentVTKStencil::TurbulentVTKStencil(const Parameters &parameters, int Nx, int Ny, int Nz)
        : VTKStencil(parameters, Nx, Ny, Nz)
        , eddyViscosity_(ScalarField(Nx, Ny, parameters.geometry.dim == 3 ? Nz : 1))
        , distanceToWall_(ScalarField(Nx, Ny, parameters.geometry.dim == 3 ? Nz : 1)) {}

    void TurbulentVTKStencil::apply(FlowField& flowField, int i, int j, int k) {
        // Apply function of VTKStencil that's used for positions, pressures and velocities
        VTKStencil::apply(flowField, i, j, k);

        // Make sure that it is a fluid cell, and if it is not, stop the computation and store 0s instead!
        if ((flowField.getFlags().getValue(i, j, k) & OBSTACLE_SELF) != 0) {
            return;
        }

        // Get the data structures stored
        FLOAT& cellEddyViscosity = eddyViscosity_.getScalar(i, j, k);
        FLOAT& cellDistanceToWall = distanceToWall_.getScalar(i, j, k);

        // Get the eddy viscosity and distance to wall
        cellEddyViscosity = flowField.getEddyViscosity().getScalar(i, j, k);
        cellDistanceToWall = flowField.getDistance().getScalar(i, j, k);
    }

    void TurbulentVTKStencil::writeEddyViscosities_(FILE* filePtr) {
        fprintf(filePtr, "SCALARS eddy_viscosity float 1\n");
        fprintf(filePtr, "LOOKUP_TABLE default\n");

        FLOAT cellEddyViscosity;
        for (auto& cellIndex : getCellIndices_()) {
            cellEddyViscosity = eddyViscosity_.getScalar(cellIndex.i, cellIndex.j, cellIndex.k);
            fprintf(filePtr, "%f\n", cellEddyViscosity);
        }

        fprintf(filePtr, "\n");
    }

    void TurbulentVTKStencil::writeDistanceToWalls_(FILE* filePtr) {
        fprintf(filePtr, "SCALARS distance float 1\n");
        fprintf(filePtr, "LOOKUP_TABLE default\n");

        FLOAT cellDistanceToWall;
        for (auto& cellIndex : getCellIndices_()) {
            cellDistanceToWall = distanceToWall_.getScalar(cellIndex.i, cellIndex.j, cellIndex.k);
            fprintf(filePtr, "%f\n", cellDistanceToWall);
        }

        fprintf(filePtr, "\n");
    }

    void TurbulentVTKStencil::writeValues_(FILE* filePtr) {
        // Write positions, pressures and velocities
        VTKStencil::writeValues_(filePtr);

        // Write eddy viscosity and distance to wall
        writeEddyViscosities_(filePtr);
        writeDistanceToWalls_(filePtr);
    }
} // namespace NSEOF::Stencils
