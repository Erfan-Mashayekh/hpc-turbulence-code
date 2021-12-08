#ifndef __PARAMETERS_HPP__
#define __PARAMETERS_HPP__

#include "Meshsize.hpp"
#include "Definitions.hpp"

#include <string>

namespace NSEOF {

//! Classes for the parts of the parameters
//@{
class TimestepParameters {
public:
    FLOAT dt;   //! Timestep
    FLOAT tau;  //! Security factor
};

class SimulationParameters {
public:
    FLOAT finalTime;        //! Final time for the simulation
    std::string type;       //! Type of the simulation (DNS vs. Turbulence)
    std::string scenario;   //! If channel or cavity, for example
};


class TurbulenceParameters{
public:
	int turb_viscosity; 	//! set a eddyturbulence to zero or not zero (=1)
	int model;		//! choose the turbulence model via an integer 0,1,2,3
};
class EnvironmentalParameters {
public:
    // Gravity components
    FLOAT gx;
    FLOAT gy;
    FLOAT gz;
};

class FlowParameters {
public:
    FLOAT Re;   //! Reynolds number
};

class SolverParameters {
public:
    FLOAT gamma;        //! Donor cell balance coefficient
    int maxIterations;  //! Maximum number of iterations in the linear solver
};

class GeometricParameters {
public:
    // Dimensions
    int dim;

    // Number of cells
    int sizeX;
    int sizeY;
    int sizeZ;

    // Cell sizing
    FLOAT lengthX;
    FLOAT lengthY;
    FLOAT lengthZ;

    // Meshsize type
    int meshsizeType;

    // For mesh-stretching
    int stretchX;
    int stretchY;
    int stretchZ;
};

class WallParameters {
public:
    // Scalar value definition. Used to define the pressure, for example.
    FLOAT scalarLeft;
    FLOAT scalarRight;
    FLOAT scalarBottom;
    FLOAT scalarTop;
    FLOAT scalarFront;
    FLOAT scalarBack;

    // Vector values at the boundaries, to define, for example, the velocities.
    FLOAT vectorLeft[3];
    FLOAT vectorRight[3];
    FLOAT vectorBottom[3];
    FLOAT vectorTop[3];
    FLOAT vectorFront[3];
    FLOAT vectorBack[3];

    // Define how will the boundary behave
    BoundaryType typeLeft;
    BoundaryType typeRight;
    BoundaryType typeTop;
    BoundaryType typeBottom;
    BoundaryType typeFront;
    BoundaryType typeBack;
};

class VTKParameters {
public:
    const std::string vtkFileHeader = "# vtk DataFile Version 2.0\n"
                                      "I need something to put here\n"
                                      "ASCII!\n";      //! VTK File Prefix
    const std::string datasetName = "STRUCTURED_GRID"; //! Dataset Name
    const std::string outDir = "output";               //! Output directory

    //! The next offsets are set to iterate only in the white region!
    //! Since this is a bounded iterator, the real offsets become "low = 1 + 1 = 2" and "high = 1 + 0 = 1".
    const int whiteRegionLowOffset = 1;                //! Low offset for the iterator
    const int whiteRegionHighOffset = 0;               //! High offset for the iterator

    FLOAT interval;                                    //! Time interval for file printing
    std::string prefix;                                //! Output filename
};

class StdOutParameters {
public:
    FLOAT interval;
};

class ParallelParameters {
public:
    int rank;               //! Rank of the current processor

    int numProcessors[3];   //! Array with the number of processors in each direction

    //@brief Ranks of the neighbors
    //@{
    int leftNb;
    int rightNb;
    int bottomNb;
    int topNb;
    int frontNb;
    int backNb;
    //@}

    int indices[3];         //! 3D indices to locate the array
    int localSize[3];       //! Size for the local flow field
    int firstCorner[3];     //! Position of the first element. Used for plotting

#ifdef BUILD_WITH_PETSC
        PetscInt* sizes[3]; //! Arrays with the sizes of the blocks in each direction.
#else
        int* sizes[3];
#endif
};

class BFStepParameters {
public:
    FLOAT xRatio;
    FLOAT yRatio;
};


//@}

/** A class to store and pass around the parameters
 */
class Parameters {
public:
    inline Parameters()
        : meshsize(NULL) {}

    inline ~Parameters() {
        if (meshsize != NULL) {
            delete meshsize;
            meshsize=NULL;
        }
    }

    SimulationParameters    simulation;
    TimestepParameters      timestep;
    EnvironmentalParameters environment;
    FlowParameters          flow;
    SolverParameters        solver;
    GeometricParameters     geometry;
    WallParameters          walls;
    VTKParameters           vtk;
    ParallelParameters      parallel;
    StdOutParameters        stdOut;
    BFStepParameters        bfStep;
    // TODO WS2: include parameters for turbulence
    TurbulenceParameters    turbulence;
    Meshsize                *meshsize;
};

} // namespace NSEOF

#endif // __PARAMETERS_HPP__
