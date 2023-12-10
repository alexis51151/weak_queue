#! /bin/bash

# Script to run the experiments
n_cores=$(nproc --all) # Get the number of cores on the machine
n_trials=5

echo Detected "$n_cores" cores


# 1. 10M operations, from 1 to n_cores
n_iter=100
n_enqueue_dequeue=1000000

# Print header
printf "#1: memory_model\tn_threads\tn_iter\tn\tdur_ms\ttot_dur_work_ms\n"

for ((i = 0; i <= n_cores; i+= 2))
do
  # Do several trials
  for ((j = 1; j <= n_trials; j++))
  do
        ./bench "$i" "$n_iter" "$n_enqueue_dequeue"
  done
done