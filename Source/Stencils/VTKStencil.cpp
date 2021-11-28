#include "VTKStencil.hpp"
#include "StencilFunctions.hpp"
#include "Definitions.hpp"

namespace NSEOF::Stencils {

    VTKStencil::VTKStencil(const Parameters &parameters)
            : FieldStencil<FlowField>(parameters) {}

    VTKStencil::~VTKStencil() {
        for (FLOAT* velocity : velocities_) {
            free(velocity);
        }

        pressures_.clear();
        velocities_.clear();
    }

    // TODO: Include obstacles?? Erfan
    void VTKStencil::apply(FlowField& flowField, int i, int j, int k) {
        // Store the initial cell index
        if (firstCornerIdx[0] == -1) {
            firstCornerIdx[0] = i; firstCornerIdx[1] = j; firstCornerIdx[2] = k;
        }

        // Initialize data structures for pressure and velocity
        FLOAT pressure;
        auto* velocity = (FLOAT*) malloc(3 * sizeof(FLOAT));

        // Get the pressure and velocity
        if (parameters_.geometry.dim == 2) { // 2D
            flowField.getPressureAndVelocity(pressure, velocity, i, j);
            velocity[2] = 0.0;
        } else if (parameters_.geometry.dim == 3) { // 3D
            flowField.getPressureAndVelocity(pressure, velocity, i, j, k);
        } else {
            std::cerr << "This app only supports 2D and 3D geometry" << std::endl;
            exit(1);
        }

        // Store the pressure and velocity data
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

        FLOAT posZ = parameters_.parallel.firstCorner[2];

        for (int k = firstCornerIdx[2]; k < firstCornerIdx[2] + numPointsZ; k++) {
            FLOAT posY = parameters_.parallel.firstCorner[1];

            for (int j = firstCornerIdx[1]; j < firstCornerIdx[1] + numPointsY; j++) {
                FLOAT posX = parameters_.parallel.firstCorner[0];

                for (int i = firstCornerIdx[0]; i < firstCornerIdx[0] + numPointsX; i++) {
                    fprintf(filePtr, "%f %f %f\n", posX, posY, std::abs(posZ));
                    posX += parameters_.meshsize->getDx(i, j, k);
                }

                posY += parameters_.meshsize->getDy(firstCornerIdx[0] + numPointsX, j, k);
            }

            posZ += parameters_.meshsize->getDz(firstCornerIdx[0] + numPointsX, firstCornerIdx[1] + numPointsY, k);
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
        // Decide on the filename
        long time = timeStep * parameters_.vtk.interval * 1e6;
        std::string filename = parameters_.vtk.outDir + "/" + parameters_.vtk.prefix + "_" +
                               std::to_string(time) + ".vtk";

        // Open the file stream
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
