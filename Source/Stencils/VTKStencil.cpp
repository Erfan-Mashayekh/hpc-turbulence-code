#include "VTKStencil.hpp"
#include "StencilFunctions.hpp"
#include "Definitions.hpp"

namespace NSEOF::Stencils {

    PositionIdx::PositionIdx(int i, int j, int k) : i(i), j(j), k(k) {}

    VTKStencil::VTKStencil(const Parameters &parameters, int Nx, int Ny, int Nz)
            : FieldStencil<FlowField>(parameters)
            , pressure_(ScalarField(Nx, Ny, parameters.geometry.dim == 3 ? Nz : 1))
            , velocity_(VectorField(Nx, Ny, parameters.geometry.dim == 3 ? Nz : 1)) {}

    VTKStencil::~VTKStencil() {
        positionsIdxList_.clear();
    }

    // TODO: Include obstacles?? Erfan
    void VTKStencil::apply(FlowField& flowField, int i, int j, int k) {
        // Store the position indices
        positionsIdxList_.emplace_back(i, j, k);

        // Get data structures stored
        FLOAT& pressure = pressure_.getScalar(i, j, k);
        FLOAT* velocity = velocity_.getVector(i, j, k);

        // Get the pressure and velocity
        if (parameters_.geometry.dim == 2) { // 2D
            flowField.getPressureAndVelocity(pressure, velocity, i, j);
        } else if (parameters_.geometry.dim == 3) { // 3D
            flowField.getPressureAndVelocity(pressure, velocity, i, j, k);
        } else {
            std::cerr << "This app only supports 2D and 3D geometry" << std::endl;
            exit(1);
        }
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

        PositionIdx initPosIdx = positionsIdxList_[0];
        FLOAT posZ = parameters_.meshsize->getPosZ(initPosIdx.i, initPosIdx.j, initPosIdx.k);

        for (int k = initPosIdx.k; k < initPosIdx.k + numPointsZ; k++) {
            FLOAT posY = parameters_.meshsize->getPosY(initPosIdx.i, initPosIdx.j, initPosIdx.k);

            for (int j = initPosIdx.j; j < initPosIdx.j + numPointsY; j++) {
                FLOAT posX = parameters_.meshsize->getPosX(initPosIdx.i, initPosIdx.j, initPosIdx.k);

                for (int i = initPosIdx.i; i < initPosIdx.i + numPointsX; i++) {
                    fprintf(filePtr, "%f %f %f\n", posX, posY, std::abs(posZ));
                    posX += parameters_.meshsize->getDx(i, j, k);
                }

                posY += parameters_.meshsize->getDy(initPosIdx.i + numPointsX, j, k);
            }

            posZ += parameters_.meshsize->getDz(initPosIdx.i + numPointsX, initPosIdx.j + numPointsY, k);
        }

        fprintf(filePtr, "\n");
    }

    void VTKStencil::writePressures_(FILE* filePtr) {
        fprintf(filePtr, "CELL_DATA %zu\n", positionsIdxList_.size());
        fprintf(filePtr, "SCALARS pressure float 1\n");
        fprintf(filePtr, "LOOKUP_TABLE default\n");

        for (auto& positionIdx : positionsIdxList_) {
            fprintf(filePtr, "%f\n", pressure_.getScalar(positionIdx.i, positionIdx.j, positionIdx.k));
        }

        fprintf(filePtr, "\n");
    }

    void VTKStencil::writeVelocities_(FILE* filePtr) {
        fprintf(filePtr, "VECTORS velocity float\n");

        FLOAT* velocity;
        for (auto& positionIdx : positionsIdxList_) {
            velocity = velocity_.getVector(positionIdx.i, positionIdx.j, positionIdx.k);
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
