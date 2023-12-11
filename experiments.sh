#! /bin/bash

# Script to run the experiments
#n_cores=$(nproc --all) # Get the number of cores on the machine

echo Detected "$n_cores" cores

if [ $# -ne  6 ]
then
  echo "Usage: ./experiments.sh n_iter n_enqueue_dequeue n_trials step n_cores"
fi

n_iter=$1
n_enqueue_dequeue=$2
n_trials=$3
step=$4
n_cores=$5

# Print header
printf "#1: memory_model\tn_threads\tn_iter\tn\tdur_ms\ttot_dur_work_ms\n"

for ((i = 0; i <= n_cores; i+= step))
do
  # Do several trials
  for ((j = 1; j <= n_trials; j++))
  do
        ./bench "$i" "$n_iter" "$n_enqueue_dequeue"
  done
done