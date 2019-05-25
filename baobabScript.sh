#!/bin/sh
#SBATCH --job-name=miningSimulation
#SBATCH --output=out.o%j
#SBATCH --partition=shared,cui
#SBATCH --ntasks=20
#SBATCH --time=01:00:00

srun ./blockchainParallel 200 10
