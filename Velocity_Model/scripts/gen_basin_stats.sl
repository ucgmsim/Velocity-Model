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

PYTHONPATH=/home/baes/qcore:/home/baes/Velocity-Model:/home/baes/mapping
#for x in rerun/stations_????
#do
#    y=${x/stations_/tmp_out_}
#    cmd="python station_containment.py $x $y"
#    echo $cmd
#    `$cmd` &
#done
#wait
srun python gen_stats_and_basin_outlines.py

