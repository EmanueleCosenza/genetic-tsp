#!/bin/bash

graph_file=$1
pop_size=$2
cross_prob=$3
mut_prob=$4
max_gen=$5
runs=$6
max_nw=$7
seed=$8

file=test_data.csv

if [ "$#" -ne 8 ]; then
    echo "Usage: $0 graph_file pop_size cross_prob mut_prob max_gen runs max_nw seed" >&2
    exit 1
fi

echo ""
echo "Starting experiment."
echo "Printing data in ${file} (can be used later by data_analysis.py to show scalability/speedup plots)."
echo "Every version with the respective number of workers will be tested for $runs runs."
echo ""

# Start sequential runs
echo "--- Sequential version ---"
echo "Starting $runs runs."
for (( run=1; run<=$runs; run++))
do
    a=$(./tspseq $graph_file $pop_size $cross_prob $mut_prob $max_gen $seed | tail -1 | cut -d ":" -f2 | cut -d " " -f2 | tr -d "\n")
    echo "Run $run, time: $a ms"
    echo "0 $a" >> ${file}
done

echo ""

# Start parallel runs for each parallelism degree (C++ threads, active wait barrier)
echo "--- Parallel version (C++ threads, active wait barrier) ---"
for (( nw=1; nw<=$max_nw; nw*=2))
do
    if [ "$nw" -eq 1 ]; then
        echo "Starting $runs runs, $nw worker."
    else
        echo "Starting $runs runs, $nw workers."
    fi
    for (( run=1; run<=$runs; run++))
    do
        a=$(./tsppar_th $graph_file $pop_size $cross_prob $mut_prob $max_gen $nw $seed | tail -1 | cut -d ":" -f2 | cut -d " " -f2 | tr -d "\n")
        echo "Run $run, time: $a ms"
        echo "$nw $a" >> ${file}
    done
done

echo ""

# Start parallel runs for each parallelism degree (C++ threads, blocking barrier)
echo "--- Parallel version (C++ threads, blocking barrier) ---"
for (( nw=1; nw<=$max_nw; nw*=2))
do
    if [ "$nw" -eq 1 ]; then
        echo "Starting $runs runs, $nw worker."
    else
        echo "Starting $runs runs, $nw workers."
    fi
    for (( run=1; run<=$runs; run++))
    do
        a=$(./tsppar_th_block $graph_file $pop_size $cross_prob $mut_prob $max_gen $nw $seed | tail -1 | cut -d ":" -f2 | cut -d " " -f2 | tr -d "\n")
        echo "Run $run, time: $a ms"
        echo "$nw $a" >> ${file}
    done
done

echo ""

# Start parallel runs for each parallelism degree (FastFlow)
echo "--- Parallel version (FastFlow) ---"
for (( nw=1; nw<=$max_nw; nw*=2))
do
    if [ "$nw" -eq 1 ]; then
        echo "Starting $runs runs, $nw worker."
    else
        echo "Starting $runs runs, $nw workers."
    fi
    for (( run=1; run<=$runs; run++))
    do
        a=$(./tsppar_ff $graph_file $pop_size $cross_prob $mut_prob $max_gen $nw $seed | tail -1 | cut -d ":" -f2 | cut -d " " -f2 | tr -d "\n")
        echo "Run $run, time: $a ms"
        echo "$nw $a" >> ${file}
    done
done

echo ""
echo "Experiment executed successfully."
