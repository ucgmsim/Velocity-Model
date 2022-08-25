#!/bin/bash
# script version: slurm
#
# Run on Maui

#SBATCH --job-name=get_z_values
#SBATCH --account=nesi00213
#SBATCH --mem=180G
#SBATCH --time=05:00:00
#SBATCH --array=0-79%2
#SBATCH --ntasks=2              # number of tasks (e.g. MPI)
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1       # number of cores per task (e.g. OpenMP)
#SBATCH --hint=nomultithread    # don't use hyperthreading


#submit with sbatch --export=ALL

#Here, we have
#$SLURM_ARRAY_TASK_ID  0-79
#echo $SLURM_ARRAY_TASK_MIN 0
#echo $SLURM_ARRAY_TASK_MAX 79

#at a time. we have only 2 tasks runnin, but each task has its own task id 0-79

which python

i=`printf %04d $SLURM_ARRAY_TASK_ID` # 1 --> 0001
infile=basin_stats.ll_${i} # basin_stats.ll_0001
outfile=${infile/.ll/.z} # basin_stats.z_0001

indir=splits
outdir=z_values

echo $infile
echo $outfile

cmd="python get_z.py -ll $indir/$infile --nzvm-path `which NZVM` -v 2.07 -z Z1.0 Z2.5 -o $outdir/$outfile"
echo $cmd
`$cmd`


