#!/bin/bash

#SBATCH -A geo112
#SBATCH -J nzvmodgen

#SBATCH -N 25

#SBATCH -t 3:00:00


cd $SLURM_SUBMIT_DIR

date

export OMP_NUM_THREADS=16

# an error message is thrown if this is empty
rm -r Full_Velo_Mod

ulimit -u 

# run 25 copies of the MPI executable, one per node (25 nodes), and 16 threads per task
# https://docs.olcf.ornl.gov/systems/andes_user_guide.html#job-execution
srun -n25 -N25 -c16 --cpu-bind=threads ./NZVM nzvm_020m_gen.txt

#mv Full_Velo_Mod Full_Velo_Mod_20m
