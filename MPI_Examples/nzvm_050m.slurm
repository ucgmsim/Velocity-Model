#!/bin/bash

#SBATCH -A geo112
#SBATCH -J nzvmodgen

#SBATCH -N 4

#SBATCH -t 2:00:00


cd $SLURM_SUBMIT_DIR

date

export OMP_NUM_THREADS=16

# an error message is thrown if this is empty
rm -r Full_Velo_Mod

# run eight copies of the MPI executable, two per node (4 nodes total), and 16 threads per task
# https://docs.olcf.ornl.gov/systems/andes_user_guide.html#job-execution
srun -n8 -N4 -c16 --cpu-bind=threads ./NZVM nzvm_050m_gen.txt
