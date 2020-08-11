#!/bin/bash

#SBATCH -J test
#SBATCH -p cpu
#SBATCH -N 1 
#SBATCH --open-mode=append
#SBATCH -o %x.out
#SBATCH -e %x.error
#SBATCH --ntasks=1
#SBATCH --mem=1gb
#SBATCH --cpus-per-task=1
#SBATCH --comment python
#SBATCH --hint=compute_bound

opt1=$1
opt2=$2
opt3=$3
opt4=$4
opt5=$5
opt6=$6

python python/runTSelector.py $opt1 $opt2 $opt3 $opt4 $opt5 $opt6
