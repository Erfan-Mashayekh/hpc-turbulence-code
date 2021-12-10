#include "Simulation.hpp"
#include "Configuration.hpp"
#include "MeshsizeFactory.hpp"

#include "ParallelManagers/PetscParallelConfiguration.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sys/stat.h>

#include <filesystem>

int main(int argc, char *argv[]) {
    // Parallelization related. Initialize and identify.
    // ---------------------------------------------------
    int rank;   // This is the processor's identifier
    int nproc;  // Number of processors in the group
#ifdef BUILD_WITH_PETSC
    PetscInitialize(&argc, &argv, "petsc_commandline_arg", PETSC_NULL);
#else
    MPI_Init(&argc, &argv);
#endif
    MPI_Comm_size(PETSC_COMM_WORLD, &nproc);
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    std::cout << "Rank: " << rank << ", Nproc: " << nproc << std::endl;
    //----------------------------------------------------


#ifdef USE_SINGLE_PRECISION
    std::cout << "Using Single FP-Precision" << std::endl;
#else
    std::cout << "Using Double FP-Precision" << std::endl;
#endif

#ifndef NDEBUG
    std::cout << "Running in Debug Mode" << std::endl;
#else
    std::cout << "Running in Release Mode" << std::endl;
#endif

    if (!argv[1]) {
        HANDLE_ERROR(1, "You need to pass a configuration file: mpirun -np 1 build/ns ExampleCases/Cavity2D.xml");
    }

    // Read configuration and store information in parameters object
    NSEOF::Configuration configuration(argv[1]);
    NSEOF::Parameters parameters;
    configuration.loadParameters(parameters);
    NSEOF::ParallelManagers::PetscParallelConfiguration parallelConfiguration(parameters);
    NSEOF::MeshsizeFactory::getInstance().initMeshsize(parameters);
    NSEOF::FlowField* flowField = NULL;
    NSEOF::Simulation* simulation = NULL;

#ifndef NDEBUG
    std::cout << "Processor " << parameters.parallel.rank << " with index ";
    std::cout << parameters.parallel.indices[0] << ",";
    std::cout << parameters.parallel.indices[1] << ",";
    std::cout << parameters.parallel.indices[2];
    std::cout <<    " is computing the size of its subdomain and obtains ";
    std::cout << parameters.parallel.localSize[0] << ", ";
    std::cout << parameters.parallel.localSize[1] << " and ";
    std::cout << parameters.parallel.localSize[2];
    std::cout << ". Left neighbour: " << parameters.parallel.leftNb;
    std::cout << ", right neighbour: " << parameters.parallel.rightNb;
    std::cout << std::endl;
    std::cout << "Min. meshsizes: " << parameters.meshsize->getDxMin() << ", " << parameters.meshsize->getDyMin() << ", " << parameters.meshsize->getDzMin() << std::endl;
#endif

    // Initialise simulation
    if (parameters.simulation.type == "turbulence") {
        // TODO WS2: initialise turbulent flow field and turbulent simulation object
    } else if (parameters.simulation.type == "dns") {
        if (rank==0) {
            std::cout << "Start DNS simulation in " << parameters.geometry.dim << "D" << std::endl;
        }
        flowField = new NSEOF::FlowField(parameters);
        if (flowField == NULL) {
            HANDLE_ERROR(1, "flowField == NULL!");
        }
        simulation = new NSEOF::Simulation(parameters, *flowField);
    } else {
        HANDLE_ERROR(1, "Unknown simulation type! Currently supported: dns, turbulence");
    }

    // Call initialization of simulation (initialize flow field)
    if (simulation == NULL) {
        HANDLE_ERROR(1, "simulation == NULL!");
    }
    simulation->initializeFlowField();

    // flowField->getFlags().show();

    // Create the output directory (Master)
    if (rank == 0) {
        // Check if the directory already exists, and if it is not, create it!
        if (!std::filesystem::is_directory(parameters.vtk.outDir.c_str()) ||
            !std::filesystem::exists(parameters.vtk.outDir.c_str())) {
            std::filesystem::create_directory(parameters.vtk.outDir.c_str());
        }
    }

    FLOAT time = 0.0;
    FLOAT timeStdOut = parameters.stdOut.interval;
    FLOAT timeVtk = parameters.vtk.interval;
    int timeSteps = 0;

    // TODO WS1: plot initial state
    simulation->plotVTK(timeSteps++);
    

    // Time loop
    while (time < parameters.simulation.finalTime) {
        simulation->solveTimestep();

        time += parameters.timestep.dt;

        // Log the time (Master)
        if (rank == 0 && timeStdOut <= time) {
            std::cout << "Current time: " << time << "\tTimestep: " << parameters.timestep.dt << std::endl;
            timeStdOut += parameters.stdOut.interval;
        }

        // TODO WS1: trigger VTK output
        if (timeVtk <= time) {
            simulation->plotVTK(timeSteps);
            timeVtk += parameters.vtk.interval;
        }

        timeSteps++;
    }

    // TODO WS1: plot final output
    simulation->plotVTK(timeSteps);

    delete simulation;
    simulation = NULL;

    delete flowField;
    flowField = NULL;

#ifdef BUILD_WITH_PETSC
    PetscFinalize();
#else
    MPI_Finalize();
#endif

    return EXIT_SUCCESS;
}
