#! /bin/bash

# 1. Run the experiments
n_cores=$(nproc --all)

./experiments.sh 1000 10000000 5 1 n_cores| tee logs.txt

# Retrieve the tsv file

cat logs.txt | ./get-1.sh > bench.tsv

# Plot the results
Rscrit plot_results.r