#include "VTKStencil.hpp"
#include "StencilFunctions.hpp"
#include "Definitions.hpp"
#include "assert.h"

namespace NSEOF::Stencils {
    VTKStencil::VTKStencil(const Parameters& parameters)
        : FieldStencil<FlowField>(parameters) {}

    void VTKStencil::save_(FLOAT* position, FLOAT& pressure, FLOAT* velocity) {
        // Check if no ghosting cells are included!
        for (int dim = 0; dim < 3; dim++) {
            ASSERTION(0 <= position[dim] && position[dim] <= 1)
        }

        positions_.push_back(position);
        pressures_.push_back(pressure);
        velocities_.push_back(velocity);
    }

    void VTKStencil::apply(FlowField& flowField, int i, int j) {
        auto* position = (FLOAT*) malloc(3 * sizeof(FLOAT));
        FLOAT pressure;
        auto* velocity = (FLOAT*) malloc(3 * sizeof(FLOAT));

        // get position given the indices
        position[0] = parameters_.meshsize->getPosX(i, j);
        position[1] = parameters_.meshsize->getPosY(i, j);

        // set 0 for the z-axis values
        position[2] = 0.0; velocity[2] = 0.0;

        flowField.getPressureAndVelocity(pressure, velocity, i, j);
        save_(position, pressure, velocity);
    }

    void VTKStencil::apply(FlowField& flowField, int i, int j, int k) {
        auto* position = (FLOAT*) malloc(3 * sizeof(FLOAT));
        FLOAT pressure;
        auto* velocity = (FLOAT*) malloc(3 * sizeof(FLOAT));

        // get position given the indices
        position[0] = parameters_.meshsize->getPosX(i, j, k);
        position[1] = parameters_.meshsize->getPosY(i, j, k);
        position[2] = parameters_.meshsize->getPosZ(i, j, k);

        flowField.getPressureAndVelocity(pressure, velocity, i, j, k);
        save_(position, pressure, velocity);
    }

    void VTKStencil::writePositions_(FILE* filePtr) {
        fprintf(filePtr, "DATASET %s\n", parameters_.vtk.datasetName.c_str());
        fprintf(filePtr, "DIMENSIONS %d %d %d\n",
                parameters_.geometry.sizeX + 1, parameters_.geometry.sizeY + 1,
                (parameters_.geometry.dim == 2) ? 0 : parameters_.geometry.sizeZ + 1);
        fprintf(filePtr, "POINTS %d float\n",
                (parameters_.geometry.sizeX + 1) * (parameters_.geometry.sizeY + 1));

        for (auto& position : positions_) {
            fprintf(filePtr, "%f %f %f\n", position[0], position[1], position[2]);
        }

        fprintf(filePtr, "\n");
    }

    void VTKStencil::writePressures_(FILE* filePtr) {
        fprintf(filePtr, "CELL_DATA %zu\n", pressures_.size());
        fprintf(filePtr, "SCALARS pressure float 1\n");
        fprintf(filePtr, "LOOKUP_TABLE default\n");

        for (auto& pressure : pressures_) {
            fprintf(filePtr, "%f\n", pressure);
        }

        fprintf(filePtr, "\n");
    }

    void VTKStencil::writeVelocities_(FILE* filePtr) {
        fprintf(filePtr, "VECTORS velocity float\n");

        for (auto& velocity : velocities_) {
            fprintf(filePtr, "%f %f %f\n", velocity[0], velocity[1], velocity[2]);
        }

        fprintf(filePtr, "\n");
    }

    void VTKStencil::write(int timeStep) {
        // Open the file stream
        std::string filename = parameters_.vtk.outputDir + "/" + parameters_.vtk.prefix + "_" +
                               std::to_string(timeStep) + ".vtk";
        FILE* filePtr = fopen(filename.c_str(), "w");

        // Write header to the file
        fprintf(filePtr, "%s\n", parameters_.vtk.vtkFileHeader.c_str());

        // Write data to the file
        writePositions_(filePtr);
        writePressures_(filePtr);
        writeVelocities_(filePtr);

        // Close the file stream
        fclose(filePtr);
    }
} // namespace NSEOF::Stencils
