#ifndef __DEFINITIONS_HPP__
#define __DEFINITIONS_HPP__

#include <mpi.h>

#ifdef BUILD_WITH_PETSC
#include <petscksp.h>
#else
#define PETSC_COMM_WORLD MPI_COMM_WORLD
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <float.h>

// Datatype for the type of data stored in the structures
#ifdef USE_SINGLE_PRECISION
#define MY_MPI_FLOAT MPI_FLOAT
#define MY_FLOAT_MAX FLT_MAX
typedef float FLOAT;
#else
#define MY_MPI_FLOAT MPI_DOUBLE
#define MY_FLOAT_MAX DBL_MAX
typedef double FLOAT;
#endif

namespace NSEOF {

// Types of boundary
enum BoundaryType {
    DIRICHLET, // Also used for the case of non-moving wall
    PERIODIC,
    PARALLEL_BOUNDARY,
    NEUMANN
};

static constexpr int OBSTACLE_SELF =   1 << 0;
static constexpr int OBSTACLE_LEFT =   1 << 1;
static constexpr int OBSTACLE_RIGHT =  1 << 2;
static constexpr int OBSTACLE_BOTTOM = 1 << 3;
static constexpr int OBSTACLE_TOP =    1 << 4;
static constexpr int OBSTACLE_FRONT =  1 << 5;
static constexpr int OBSTACLE_BACK =   1 << 6;

// An assertion sending back a message
#ifndef NDEBUG
#define ASSERTION(boolean) \
    if (!(boolean)) { \
        std::cerr << "Assertion failed: " << #boolean << " In file " << __FILE__ << ", line " << __LINE__ << "." << std::endl; \
        exit(2); \
    }
#else
#define ASSERTION(boolean)
#endif

// An error handler, at least to check memory allocation. Active also outside debug.
#define HANDLE_ERROR(code, message) \
    { \
        std::cerr << "ERROR in file " << __FILE__ << ", line " << __LINE__ << std::endl << #message << std::endl; \
        exit(code); \
    }

} // namespace NSEOF

#endif // __DEFINITIONS_HPP__
