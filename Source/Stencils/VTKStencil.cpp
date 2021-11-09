#include "VTKStencil.hpp"
#include "StencilFunctions.hpp"
#include "Definitions.hpp"
#include "assert.h"

namespace NSEOF::Stencils {
    VTKStencil::VTKStencil(const Parameters& parameters)
            : FieldStencil<FlowField>(parameters) {
        firstCornerInd_ = nullptr;
        firstCornerPos_ = nullptr;
    }

    void VTKStencil::setFirstCorner_(int i, int j, int k = 0) {
        firstCornerInd_ = (int*) malloc(3 * sizeof(int));
        firstCornerPos_ = (FLOAT*) malloc(3 * sizeof(FLOAT));

        firstCornerInd_[0] = i;
        firstCornerInd_[1] = j;
        firstCornerInd_[2] = k;

        firstCornerPos_[0] = (parameters_.geometry.dim == 2)
                ? parameters_.meshsize->getPosX(i, j) : parameters_.meshsize->getPosX(i, j, k);
        firstCornerPos_[1] = (parameters_.geometry.dim == 2)
                ? parameters_.meshsize->getPosY(i, j) : parameters_.meshsize->getPosY(i, j, k);
        firstCornerPos_[2] = (parameters_.geometry.dim == 2)
                ? 0 : parameters_.meshsize->getPosZ(i, j, k);
    }

    void VTKStencil::apply(FlowField& flowField, int i, int j, int k) {
        // Init data structures
        FLOAT pressure;
        auto* velocity = (FLOAT*) malloc(3 * sizeof(FLOAT));

        // Set the starting position, the first corner!
        if (firstCornerInd_ == nullptr) {
            setFirstCorner_(i, j, k);
        }

        // Get the pressure and velocity
        if (parameters_.geometry.dim == 2) { // 2D
            flowField.getPressureAndVelocity(pressure, velocity, i, j);

            // Set z-velocity to 0!
            velocity[2] = 0.0;
        } else if (parameters_.geometry.dim == 3) { // 3D
            flowField.getPressureAndVelocity(pressure, velocity, i, j, k);
        } else {
            std::cerr << "This app only supports 2D and 3D geometry" << std::endl;
            exit(1);
        }

        // Store the data
        pressures_.push_back(pressure);
        velocities_.push_back(velocity);
    }

    void VTKStencil::apply(FlowField& flowField, int i, int j) {
        apply(flowField, i, j, 0);
    }

    void VTKStencil::writePositions_(FILE* filePtr) {
        const int numPointsX = parameters_.geometry.sizeX + 1;
        const int numPointsY = parameters_.geometry.sizeY + 1;
        const int numPointsZ = parameters_.geometry.dim == 2 ? 1 : parameters_.geometry.sizeZ + 1;

        fprintf(filePtr, "DATASET %s\n", parameters_.vtk.datasetName.c_str());
        fprintf(filePtr, "DIMENSIONS %d %d %d\n", numPointsX, numPointsY, numPointsZ);
        fprintf(filePtr, "POINTS %d float\n", numPointsX * numPointsY * numPointsZ);

        FLOAT posZ = firstCornerPos_[2];

        for (int i = -1, j = -1, k = firstCornerInd_[2]; k < firstCornerInd_[2] + numPointsZ; k++) {
            FLOAT posY = firstCornerPos_[1];

            for (j = firstCornerInd_[1]; j < firstCornerInd_[1] + numPointsY; j++) {
                FLOAT posX = firstCornerPos_[0];

                for (i = firstCornerInd_[0]; i < firstCornerInd_[0] + numPointsX; i++) {
                    fprintf(filePtr, "%f %f %f\n", posX, posY, posZ);
                    posX += parameters_.meshsize->getDx(i, j, k);
                }

                posY += parameters_.meshsize->getDy(i, j, k);
            }

            posZ += parameters_.meshsize->getDz(i, j, k);
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
        std::string filename = parameters_.vtk.outDir + "/" + parameters_.vtk.prefix + "_" +
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
