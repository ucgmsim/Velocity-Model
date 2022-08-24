#!/bin/bash
# script version: slurm
#
# must be run with sbatch c2o.sl [realisationDirectory] [managementDBLocation]

#SBATCH --job-name=gen_basin_stats
#SBATCH --account=nesi00213
#SBATCH --partition=nesi_research
#SBATCH --time=24:00:00
##SBATCH --mem-per-cpu=4800      # memory/cpu (in MB)
#SBATCH --ntasks=1340              # number of tasks (e.g. MPI)
#SBATCH --cpus-per-task=1       # number of cores per task (e.g. OpenMP)
#SBATCH --hint=nomultithread    # don't use hyperthreading

export PYTHONPATH=`pwd`/../../
srun python gen_basin_stats.py

