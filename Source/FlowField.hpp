#ifndef __FLOW_FIELD_HPP__
#define __FLOW_FIELD_HPP__

#include "Parameters.hpp"
#include "DataStructures.hpp"

namespace NSEOF {

/** Flow field
 *
 * Class intended to contain the state of the domain.
 */
class FlowField {
private:
    const int sizeX_; //! Size in the X direction
    const int sizeY_; //! Size in the Y direction
    const int sizeZ_; //! Size in the Z direction

    const int cellsX_;
    const int cellsY_;
    const int cellsZ_;

    ScalarField pressure_; //! Scalar field representing the pressure
    VectorField velocity_; //! Multicomponent field representing velocity

	ScalarField distance_to_wall_; //ws2 distance to nearest wall data structure
	
    IntScalarField flags_; //! Integer field for the flags

    VectorField FGH_;
    ScalarField RHS_;      //! Right hand side for the Poisson equation
	

public:
    /** Constructor for the 2D flow field
     *
     * Constructor for the flow field. Allocates all the fields and sets
     * the sizes. Currently, this contructor is only used for testing purposes.
     *
     * @param Nx Size of the fuild domain (non-ghost cells), in the X direction
     * @param Ny Size of the fuild domain (non-ghost cells), in the Y direction
     */
    FlowField(int Nx, int Ny);

    /** Constructor for the 3D flow field
     *
     * Constructor for the flow field. Allocates all the fields and sets
     * the sizes. Currently, this contructor is only used for testing purposes.
     *
     * @param Nx Size of the fuild domain (non-ghost cells), in the X direction
     * @param Ny Size of the fuild domain (non-ghost cells), in the Y direction
     * @param Nz Size of the fuild domain (non-ghost cells), in the Z direction
     */
    FlowField(int Nx, int Ny, int Nz);

    /** Constructs a field from parameters object
     *
     * Constructs a field from a parameters object, so that it dimensionality can be defined in
     * the configuration file.
     *
     * @param parameters Parameters object with geometric information
     */
    FlowField(const Parameters& parameters);

    virtual ~FlowField() = default;

    /** Obtain size in the X direction
     *
     * @return Number of cells in the X direction
     */
    int getNx() const;

    /** Obtain size in the Y direction
     *
     * @return Number of cells in the Y direction
     */
    int getNy() const;

    /** Obtain size in the Z direction
     *
     * @return Number of cells in the Z direction
     */
    int getNz() const;

    int getCellsX() const;
    int getCellsY() const;
    int getCellsZ() const;

    ScalarField& getPressure();
    VectorField& getVelocity();
    
    ScalarField& getDistance(); //ws2 function for nearest wall distance

    IntScalarField& getFlags();

    VectorField& getFGH();

    ScalarField& getRHS();

    void getPressureAndVelocity(FLOAT &pressure, FLOAT* const velocity, int i, int j);
    void getPressureAndVelocity(FLOAT &pressure, FLOAT* const velocity, int i, int j, int k);
};

} // namespace NSEOF

#endif // __FLOW_FIELD_HPP__
