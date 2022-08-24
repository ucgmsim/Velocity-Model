#!/bin/bash
# script version: slurm
#

#SBATCH --job-name=get_z_values
#SBATCH --account=nesi00213
#SBATCH --time=05:00:00
#SBATCH --mem-per-cpu=4G      # memory/cpu (in MB)
#SBATCH --ntasks=160              # number of tasks (e.g. MPI)
#SBATCH  --nodes=8
#SBATCH --cpus-per-task=1       # number of cores per task (e.g. OpenMP)
#SBATCH --hint=nomultithread    # don't use hyperthreading


#submit with sbatch --export=ALL

which python 

for x in splits/basin_stats.ll_????
do
    y=${x/.ll/.z}
    cmd="python get_z.py -ll $x --nzvm-path `which NZVM` -v 2.07 -z Z1.0 Z2.5 -o z_values/$y"
    echo $cmd
    `$cmd` &
done
wait


