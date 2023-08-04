# Velocity Model - MPI Branch

The MPI branch of the velocity model is based on the parallel (OpenMP) version of the code.  It is compiled using

	make mpi

It is intended for use on a multi-node cluster.  On a single node, no fundamental difference exists to the OpenMP version.  
Use of multiple nodes (MPI) is only supported for call type `GENERATE_VELOCITY_MOD`; for other call types the code should work
but must use just one MPI rank.

On a multi-node cluster, the code runs as a hybrid MPI-OpenMP code, with one MPI rank running per node.  The additional CPU cores
on each node are taking part on the computational aspects using OpenMP.  

Because the parallelization runs along the vertical (`Z`) axis, the number of of processes (ranks) must divide the number of depth levels (`NZ`).

The selection `MODEL_FORMAT` has been added to the input text file.  It can take the value `GENERIC`, in which case 
the code just outputs one slice per rank, spawning `NZ / ncpus` depth levels.  For `MODEL_FORMAT=AWP`, the output file is stored in AWP format.

The directory MPI_Examples contains example SLURM submission scripts and model configuration files to create velocity meshes with the following resolutions:

Grid spacing | NX | NY |NZ
--|--|--|--
100 m |  6000 | 4000 | 30
50 m |  12000 | 8000 | 60
20 m |  24000 | 16000 | 120

They only extend down to 3 km depth and represent the uppermost, finest mesh in the discontinuous mesh code.  Deeper meshes can be created by adjusting the spacing and beginning / ending depth.


## Notes:

- `GLOBAL_MESH->nZ` calculation was modified to include the depth level indicated by `zMax` (line 60 in [`generateModelGrid.c`](src/generateModelGrid.c).
E.g., if `zMin = 0`, `zMax = 9 * hDep`, `nZ` is 10.  This is needed to avoid missing depth levels for more than one MPI rank.

- LON_GRID_DIM_MAX and LAT_GRID_DIM_MAX were adjusted.  Since they control variables allocated statically, they should be made just large enough to hold the required model size.0jjG

- The configuration file is being read just once by rank 0 and the parameters are being broadcast to the other processes.  The same is not yet done for all the geophysical properties read from file, which can potentially slow down the code significantly on large busy parallel filesystems, in particular if a large number of MPI processes are used.  Future versions should read the info only once and then broadcast it to limit I/O overhead.

- Progress info (on the information being read) is currently written to stdout for every MPI rank.  
