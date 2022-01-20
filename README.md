# Code for Turbulent Flow Simulation on HPC-Systems (IN2311, MW2048)

This code has been tested under Ubuntu 20.04. Other Linux Distributions will probably work. However, we do not recommend using Windows or MacOS. If you do not have Linux installed on your computer, please use a Virtual Machine or similar.

## Dependencies

### MPI (recommended OpenMPI)

* Under Ubuntu you can simply run `apt install libopenmpi-dev` to install OpenMPI.

### PETSc

* Please install [PETSc](https://petsc.org/release/) on your system.

**Recommended: Use PETSc from Ubuntu 20.04 package repositories** 

We recommend to use `apt install libpetsc3.12-dev-common` to install PETSc 3.12 under Ubuntu 20.04. See https://packages.ubuntu.com/focal/libpetsc3.12-dev-common

**Alternative: Building PETSc from source**

You may also use other PETSc Versions. But this might break. This code should work with PETSc 3.7.4 or newer. If not, feel free to open an issue.

If you are building PETSc from source, the specific PETSc version might be incompatible with your system MPI. To fix this issue, please make sure to use the following options, when configuring PETSc: 

```
python2 ./configure --download-mpich --download-f2cblaslapack
```

This will tell PETSc to take care of downloading a compatible MPI version. Additionally, please set the following environment variables before running CMake:

```
export PETSC_DIR=<Folder Containing PETSc>/petsc-x.y.z/
export PETSC_ARCH=arch-linux2-c-debug
export MPI_HOME=${PETSC_DIR}/${PETSC_ARCH}
```

When running `cmake ..`, please check the output to ensure that the correct PETSc version and MPI is used. You should see something like this:

```
/hpc_turbulence_code/build$ cmake ..
...
-- Detecting additional PETSc prefixes
-- Detected <Folder Containing PETSc>/petsc-x.y.z/arch-linux2-c-debug
-- Skipped the following invalid prefixes: <Folder Containing PETSc>/petsc-x.y.z/
-- pkg-config will search the following paths:
CMAKE_PREFIX_PATH
   <Folder Containing PETSc>/petsc-x.y.z/arch-linux2-c-debug
-- Checking for module 'PETSc'
--   Found PETSc, version x.y.z
-- Found MPI_CXX: <Folder Containing PETSc>/petsc-x.y.z/arch-linux2-c-debug/lib/libmpicxx.so (found version "a.b") 
-- Found MPI: TRUE (found version "a.b")  
-- Found PETSc: TRUE (found version "x.y.z") 
-- Configuring done
-- Generating done
-- Build files have been written to: /hpc_turbulence_code/build

```

If you are building PETSc from source, make sure to use the correct `mpirun`, when running the program! Use `$MPI_HOME/bin/mpirun`, not just `mpirun`. 

### Eigen

To run the project, you have to install the Eigen library. Execute the following commands in the root directory:

- Create a library directory: `mkdir lib`
- Switch to this directory: `cd lib`
- Clone Eigen: `git clone git@github.com:libigl/eigen.git eigen`
- Switch to eigen directory: `cd eigen`
- Create build directory: `mkdir build`
- Switch to this directory: `cd build`
- Set eigen install dir in cmake (**cmake-gui** can come in handy):
  - `sudo apt-get install cmake-gui`
  - `cmake-gui`
  - Set **CMAKE_INSTALL_PREFIX** to `<project_root_dir_here>/lib/eigen`
- Then make the Eigen project by running the following commands in `<project_root_dir_here>/lib/eigen/build`: `make && make install`
- `<project_root_dir_here>/lib/eigen/` folder should contain both `share` and `include`. If not, it means that the Eigen is installed on the system directly. In that case, you have to copy `share` and `include` to `<project_root_dir_here>/lib/eigen/` or set the **EIGEN_PARENT_DIR** in the project's CMake to make it point to the folder containing  `share` and `include`.

### CMake

Run `apt install cmake` to install CMake on your system.

## Dockerfile

You can also use the Dockerfile included in the repository. This will create an image where everything is set up and you can just begin with compiling the code.

Of course, you can also just look at the Dockerfile as a recipe on how to set up your environment.

## The Code

### Compilation

As build system configurator we use CMake. To compile the code execute the following commands in this directory:

* Create a build directory: `mkdir build`
* Switch to this directory: `cd build`
* (Optional): Choose the compiler being used (if you want to use a specific MPI compiler/version): `export CXX=mpic++`
* Run CMake: `cmake ..` (this configures a Release build, which is default. For a Debug build run `cmake .. -DCMAKE_BUILD_TYPE=Debug`)
* Run Make: `make` (or `make -j` for compiling with multiple cores)
* (Optional): Run `make test` to validate your build (this will execute some unit tests and run some simulations)

### Testing
In the code skeleton, some basic unit tests have been implemented (`make test`). Feel free to add your own test cases inside the `Tests` folder.

### Running a Simulation

* Run the code in serial via `./build/ns path/to/your/configuration`
   * Example: `./build/ns ExampleCases/Cavity2D.xml` (for a Debug build, the executable is `./build/nsd`)
* Run the code in parallel via `mpirun -np n_proc ./build/ns path/to/your/configuration`
   * Example: `mpirun -np 4 ./build/ns ExampleCases/Cavity2DParallel.xml` (in the skeleton version of the code this is expected to **not work**!)

### Adding New Source Files

You can add new source files by just creating them somewhere within the `Source` folder. CMake automatically detects these files and adds them to the build.

# Development Hints & FAQ

## What does the statement `#ifndef...` in the very first line of each `*.hpp` file mean?

Probably you are talkling about the *include guard*. [More information can be found here](https://en.wikipedia.org/wiki/Include_guard). 

Note: If you forget to rename the include guard, when you copy-paste an existing class, the new class will not be compiled properly, since the class is not included properly. It is guarded by the include guard, even though it should not. 

Example: If you used `RHSStencil.hpp` as a template your original include guard will be 

```
#ifndef __RHS_STENCIL_HPP_
#define __RHS_STENCIL_HPP_
```

for `DPStencil.hpp`, you should change it to

```
#ifndef __DP_STENCIL_HPP__
#define __DP_STENCIL_HPP__
```

## It does not compile and everything seems fine

Make sure to use `make clean` before you use `make`. Sometimes there are build artifacts from previous build processes that spoil your current compilation process. `make clean` takes care of deleting everything that should not be there and allows the compiler to start from scratch.

Sometimes it is also helpful to delete the `build` folder and create a new one, following the steps from the Compilation section above.

## How can I see all the compiler flags the generated Makefile is using?

Instead of using `make`, run `make VERBOSE=1`. You can also run `make -n` to invoke a dry run where you see what the Makefile would do in case of compilation.

## How can I see the test output?

Instead of using `make test`, run `ctest --verbose`.
