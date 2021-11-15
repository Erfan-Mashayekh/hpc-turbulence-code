## Turbulent Flow Simulation on HPC-Systems - Worksheet 1: Getting Started

**| Name:** _Batuhan Erden_ **|** **Matriculation Number:** _03738750_ **|**

#### Part 1: VTK Stencil

In the **apply** function, given a cell, we are expected to get the pressure (_at the center of the cell_) and velocity values (_average across the cell_) from **Flow Field**. After getting those values, I stored them in the object by making use of the **vector** data structure in order to access the values of the particular cells during the **write** operation. I also needed to know the **First Corner**, which is the start of the _White Region_. For that reason, I simply kept the first iterated _cell indexes_. You can find the implementation of the **apply** function [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/blob/2472ab6961f6aef8f54b57d85470a64ac32e2f03/Source/Stencils/VTKStencil.cpp).

As a second step, I have implemented the **write** functionality, which writes **VTK** files with a time information (in _microseconds_) appended to the filename. For that I have created separate function for _writing the nodes/points_, _writing the pressures_ and _writing the velocities_ for readability purposes. For keeping this report as short as possible, I decided not to include the code blocks. However, feel free to check it [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/blob/2472ab6961f6aef8f54b57d85470a64ac32e2f03/Source/Stencils/VTKStencil.cpp)! 

_**Keep in mind** that we have more nodes than we have cell values because **VTK** differentiates between vertex (or node) and cell values, and we use quadrilateral or octahedral cells on a uniform grid. For example, if we had **20 * 10 = 200** cells in a **2D** space, we would have **(20 + 1) * (10 + 1) = 231** nodes._ 

The functions **getPosX** and **getDx** are utilized to write the points.

#### Part 2: Handling the Ghost Cells and always working in the White Region:

As you might remember from the lecture, we have boundaries around our workable region (_for ease of coding and parallelization_). As can be seen in the picture [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/blob/b3d2defa9ff808e20480ab7326e2cef8dc391b61/Doc/WS1/data/img/iterator_explained.png), in addition to the _default boundary_ where the workable region is covered by a single cell space, we also have some additional ghost cells that we need to take care. For that reason, I have used the **lowOffset** and **highOffset** parameters of the **Iterator** class, which is _bounded iteration_ where by default, the _1-layer of default boundary_ is already handled. That's why instead of setting **lowOffset** to **2** and **highOffset** to **1**, I set them **1** and **0** respectively. After setting those values correctly, the **Iterator** now iterates only in the _White Region_.

#### Part 3: Plotting the VTK when the "time" is right!

- **dt_convergence** = adaptive time stepping, always changing.

- **dt_vtk** = time step size given in the parameters.

- **dt**, the actual time step.

We always wish things that cannot always happen, like having **dt_convergence** and **dt_vtk** same all the time! Since we are required to plot **every 0.01 seconds (or whatever is set in the parameter)**, I had to handle the cases where **dt_convergence** and **dt_vtk** are not the same:

##### 1) dt_convergence = dt_vtk

Everything works as expected!

##### 2) dt_convergence > dt_vtk

**dt_vtk** acts as an upper bound for **dt**. We simply set **dt** = **dt_vtk** here.

##### 3) dt_convergence < dt_vtk

The most problematic case. In this case, we are doing as many time steps as we can until we reach the next point in time to plot.

#### Part 4: The results on ParaView

I have used ParaView to check my results. I have both printed 2D and 3D and analyzed both pressure and velocity. To keep the report short, I have not included any images here. Please check [here for pressure](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/tree/475535abbe5a111868121e2e216241d0d395b41d/Doc/WS1/data/out/pressure) and [here for velocity](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/tree/475535abbe5a111868121e2e216241d0d395b41d/Doc/WS1/data/out/velocity).

#### Part 5: Flow Physics and Profiling (Questions Answered)

##### 1) Study of the scenarios cavity, channel and channel with backward-facing step in more detail:

- _**The influence of the Reynolds number on the velocity and pressure field. In particular,**_

  - _**How many vortices can you observe in the cavity scenario, depending on the choice of the Reynolds number? Where are these vortices located?**_

    As we increase the _Reynolds number_, the vorticity distribution seems more homogeneous. That is, the vortices are distributed more evenly throughout the surface. In contrast, lower _Reynolds number_ resulted in more inhomogeneous distribution where the vortices lie mostly around the borders.

  - _**How does the velocity/pressure field of the channel flow depend on the Reynolds number?**_

    As can be seen in the graph [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/blob/b3d2defa9ff808e20480ab7326e2cef8dc391b61/Doc/WS1/data/img/channel_vel-press_re.png) where **Velocity (Orange)** is plotted over **Pressure (Red)** with  the _Reynolds number_ set to **500 (Left)** and **100 (Right)**, it is not hard to deduce that setting the _Reynolds number_ high resulted in a higher **velocity/pressure field** as **velocity** values got bigger while **pressure** values got smaller. In a nutshell, **velocity/pressure field** and the _Reynolds number_ has a positive correlation.

  - _**How is the velocity/pressure field behind the backward-facing step affected by the choice of the Reynolds number?**_

    As can be seen in the graph [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/blob/b3d2defa9ff808e20480ab7326e2cef8dc391b61/Doc/WS1/data/img/backward_vel-press_re.png), the results look similar with that of channel flow. Again, we can say that **velocity/pressure field** and the _Reynolds number_ has a positive correlation.

- _**The influence of the length and height of the backward-facing step on the flow field.**_

  As can be seen in the screenshot [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/blob/b3d2defa9ff808e20480ab7326e2cef8dc391b61/Doc/WS1/data/img/backward_length-height.png), I have tested different length and height settings of the backward-facing step. It definitely has an influence on the flow field. When we look at the flow field at the top (the default one), we can see that the pressure is getting closer to **0.8**. However, when we look at the ones at the bottom, in both cases, the pressure gets to a point higher than default one.

  On the bottom left, we have smaller length and height values. As can be seen easily, the pressure is getting closer to even **1.0** whereas on the bottom right, where we have larger length and height values, the pressure was lower (close to **0.9**).

- _**The influence of the used mesh on the overall accuracy and time step size. You may restrict your considerations to the channel flow scenario for this sub-task.**_

  The mesh structure used has a significant effect on the overall accuracy and time step size. One can see [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/blob/b3d2defa9ff808e20480ab7326e2cef8dc391b61/Doc/WS1/data/img/channel_mesh.png) how the **stretched mesh (left)** converged faster (_More accurate!_) than **uniform mesh (right)**. According to the time step, **stretched mesh (left)** has a **dt** of _0.000228466_ whereas **uniform mesh (right)** has a **dt** of _0.0496032_, which is higher.

##### 2) Investigation of the sequential performance using gprof:

As can be seen in the output of the **gprof** command (_Performance analysis for Cavity 2D_) [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/blob/b3d2defa9ff808e20480ab7326e2cef8dc391b61/Doc/WS1/data/gprof/analysis_Cavity2D.txt) (for more **gprof** results, see [here](https://gitlab.lrz.de/erdenbatuhan/hpc_turbulence_code_skeleton/-/tree/b3d2defa9ff808e20480ab7326e2cef8dc391b61/Doc/WS1/data/gprof)), the case is not as expected. Actually, the **write** routine takes absolutely no time compared to the other routines. I ran the performance analysis for each of the example cases, and the results are not that different and the routines that takes most of the time are the same.

For the Cavity2D case, the **apply** routine took _37,50%_ of the time followed by **getVelocity** that took _25,00%_ of the time. When I checked my implementation of the **apply** routine and the implementation of **getVelocity**, I saw no possible improvements since I believe they are implemented in the best way possible. However, as you can see under the _calls_ section, there are too many calls to those routines. **apply** is called **231231** time and **getVelocity**, which gets called under **apply**, is called **3051048** times. So, we immediately can see there is room for improvement. We can for example make use of the **MPI** to parallelize the simulation so that multiple processes work on those _bottleneck_ routines.


I thank you for the opportunity of gaining hands-on experience with such algorithms, and I truly appreciate your effort.


#### _Batuhan Erden_

