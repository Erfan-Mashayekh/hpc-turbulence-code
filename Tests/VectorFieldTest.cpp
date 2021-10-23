#include "DataStructures.hpp"

constexpr auto SIZE_X = 10;
constexpr auto SIZE_Y = 10;
constexpr auto SIZE_Z = 10;

bool compareVectorsFails(FLOAT* v1, FLOAT* v2, int dim = 2) {
    ASSERTION((dim == 2) || (dim == 3));
    for (int i = 0; i < dim; i++) {
        if (v1[i] != v2[i]) {
            return true;
        }
    }
    return false;
}

void setEntry(FLOAT* entry, FLOAT val, int dim = 2) {
    entry[1] = val;
    entry[0] = val - 1.0 / 3;
    if (dim == 3) {
        entry[2] = val + 1.0 / 3;
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << "Testing vector fields" << std::endl;

    FLOAT entry[3];

    NSEOF::VectorField vfield2D(SIZE_X, SIZE_Y);
    NSEOF::VectorField vfield3D(SIZE_X, SIZE_Y, SIZE_Z);

    FLOAT d2counter = 1.0;
    FLOAT d3counter = 1.0;

    // Fill the arrays with data
    for (int i = 0; i < SIZE_X; i++){
        for (int j = 0; j < SIZE_Y; j++) {
            setEntry(entry, d2counter);
            vfield2D.getVector(i, j)[0] = entry[0];
            vfield2D.getVector(i, j)[1] = entry[1];
            d2counter ++;
            for (int k = 0; k < SIZE_Z; k++) {
                setEntry(entry, d3counter, 3);
                vfield3D.getVector(i, j, k)[0] = entry[0];
                vfield3D.getVector(i, j, k)[1] = entry[1];
                vfield3D.getVector(i, j, k)[2] = entry[2];
                d3counter ++;
            }
        }
    }

    d2counter = 1.0;
    d3counter = 1.0;

    // Read the data out
    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
            setEntry(entry, d2counter);
            if (compareVectorsFails(entry, vfield2D.getVector(i, j))) {
                std::cerr << "Test for 2D vector failed" << std::endl;
                return EXIT_FAILURE;
            }
            d2counter ++;

            for (int k = 0; k < SIZE_Z; k++) {
                setEntry(entry, d3counter, 3);
                if (compareVectorsFails(entry, vfield3D.getVector (i, j, k), 3)) {
                    std::cerr << "Test for 3D vector failed" << std::endl;
                    return EXIT_FAILURE;
                }
                d3counter ++;
            }
        }
    }

    std::cout << "Test for vector fields completed successfully" << std::endl;

    return EXIT_SUCCESS;
}
