#include "FlowField.hpp"
#include "Iterators.hpp"
#include "DataStructures.hpp"
#include "Stencils/StencilFunctions.hpp"

constexpr auto SIZE_X = 20;
constexpr auto SIZE_Y = 25;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << "Testing derivatives" << std::endl;

    NSEOF::Parameters parameters;
    parameters.solver.gamma = 0.5;
    FLOAT lv[81]; // Local velocity
    FLOAT lm[81]; // Local meshsize

    for (int i = 0; i < 81; i++) {
        lv[i] = 0;
        lm[i] = 1;
    }

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            for (int k = -1; k < 2; k++) {
                for (int c = 0; c < 3; c++) {
                    lv[NSEOF::Stencils::mapd(i, j, k, c)] = 1;
                }
            }
        }
    }

    for (int i = 0; i < 81; i++) {
        if (!lv[i]) {
            std::cerr << "Failed accessing all elements from coordinates and components" << std::endl;
            return EXIT_FAILURE;
        }
    }

    lv[NSEOF::Stencils::mapd(1, 0, 0, 0)] = 3;
    lv[NSEOF::Stencils::mapd(0, 0, 0, 0)] = 2;
    lv[NSEOF::Stencils::mapd(-1, 0, 0, 0)] = 1;

    // TODO: and here it does something odd.

    std::cout <<
        "dudx:      " << NSEOF::Stencils::dudx(lv, lm) << std::endl <<
        "dvdy:      " << NSEOF::Stencils::dvdy(lv, lm) << std::endl <<
        "dwdz:      " << NSEOF::Stencils::dwdz(lv, lm) << std::endl <<

        "d2udx2:    " << NSEOF::Stencils::d2udx2(lv, lm) << std::endl <<
        "d2udy2:    " << NSEOF::Stencils::d2udy2(lv, lm) << std::endl <<
        "d2udz2:    " << NSEOF::Stencils::d2udz2(lv, lm) << std::endl <<
        "d2vdx2:    " << NSEOF::Stencils::d2vdx2(lv, lm) << std::endl <<
        "d2vdy2:    " << NSEOF::Stencils::d2vdy2(lv, lm) << std::endl <<
        "d2vdz2:    " << NSEOF::Stencils::d2vdz2(lv, lm) << std::endl <<
        "d2wdx2:    " << NSEOF::Stencils::d2wdx2(lv, lm) << std::endl <<
        "d2wdy2:    " << NSEOF::Stencils::d2wdy2(lv, lm) << std::endl <<
        "d2wdz2:    " << NSEOF::Stencils::d2wdz2(lv, lm) << std::endl <<

        "duvdx:     " << NSEOF::Stencils::duvdx(lv, parameters, lm) << std::endl <<
        "duvdy:     " << NSEOF::Stencils::duvdy(lv, parameters, lm) << std::endl <<
        "duwdx:     " << NSEOF::Stencils::duwdx(lv, parameters, lm) << std::endl <<
        "duwdz:     " << NSEOF::Stencils::duwdz(lv, parameters, lm) << std::endl <<
        "dvwdy:     " << NSEOF::Stencils::dvwdy(lv, parameters, lm) << std::endl <<
        "dvwdz:     " << NSEOF::Stencils::dvwdz(lv, parameters, lm) << std::endl <<

        "du2dx:     " << NSEOF::Stencils::du2dx(lv, parameters, lm) << std::endl <<
        "dv2dy:     " << NSEOF::Stencils::dv2dy(lv, parameters, lm) << std::endl <<
        "dw2dz:     " << NSEOF::Stencils::dw2dz(lv, parameters, lm) << std::endl;

    std::cout << "Test for derivatives completed successfully" << std::endl;

    return EXIT_SUCCESS;
}
