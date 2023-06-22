#!/bin/bash

#SBATCH -A geo112
#SBATCH -J nzvmodgen

#SBATCH -N 1

#SBATCH -t 0:15:00


cd $SLURM_SUBMIT_DIR

date

export OMP_NUM_THREADS=16

# an error message is thrown if this is empty
rm -r Full_Velo_Mod

# run one copy of the MPI executable, one per node (1 node total), and 32 threads per task
# https://docs.olcf.ornl.gov/systems/andes_user_guide.html#job-execution
srun -n1 -N1 -c32 --cpu-bind=threads ./NZVM nzvm_100m_gen.txt

mv Full_Velo_mod Full_Velo_Mod_100m
