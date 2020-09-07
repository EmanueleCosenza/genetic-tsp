#!/bin/bash

dir=$1
explist=$2
mode=$3
runs=$4
max_nw=$5
seed=$6

# 1 seq, 4 par
N_VERSIONS=5

graph_file=complete.csv
pop_size=3000
cross_prob=0.9
mut_prob=0.1
max_gen=20

# Arguments
if [ "$#" -ne 6 ]; then
    echo "Usage: $0 dir explist mode runs max_nw seed" >&2
    exit 1
fi

# Remove dir and create new one
if [ -d "$dir" ]; then
    rm -r "$dir"
fi
mkdir "$dir"

# Create list of number of workers
nwlist=()
if [ "$mode" -eq 0 ]; then
    # Sequential nws
    nwlist=$(seq $max_nw)
else
    # Experiment only with nws = powers of 2
    for (( nw=1; nw<=$max_nw; nw*=2))
    do
        nwlist+=("$nw")
    done
fi

# for a in ${nwlist[@]}
# do
#     echo $a
# done

declare -a exps=($(for i in $(seq $N_VERSIONS); do echo 0; done))
for i in $(echo $explist | sed "s/,/ /g")
do
    exps["$i"]=1
done

echo ""
echo "Starting experiment."
echo "Using $dir directory to output data files (can be used later by data_analysis.py to show scalability/speedup plots)."
echo "Every version of the algorithm with the respective number of workers will be tested for $runs runs."
echo ""

if [ ${exps[0]} -eq 1 ]; then
    # Start sequential runs
    echo "--- Sequential version ---"
    echo "Starting $runs runs."
    for (( run=1; run<=$runs; run++))
    do
        a=$(./tspseq $graph_file $pop_size $cross_prob $mut_prob $max_gen $seed | tail -1 | cut -d ":" -f2 | cut -d " " -f2 | tr -d "\n")
        echo "Run $run, time: $a ms"
        echo "0 $a" >>  $dir/seq.csv
    done
    echo ""
fi

if [ ${exps[1]} -eq 1 ]; then
    # Start parallel runs for each parallelism degree (C++ threads, spin barrier)
    echo "--- Parallel version (C++ threads, active wait barrier) ---"
    for nw in ${nwlist[@]}
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
            echo "$nw $a" >>  $dir/spinbar.csv
        done
    done
    echo ""
fi

if [ ${exps[2]} -eq 1 ]; then
    # Start parallel runs for each parallelism degree (C++ threads, blocking barrier)
    echo "--- Parallel version (C++ threads, blocking barrier) ---"
    for nw in ${nwlist[@]}
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
            echo "$nw $a" >> $dir/blockbar.csv
        done
    done
fi

echo ""

if [ ${exps[3]} -eq 1 ]; then
    # Start parallel runs for each parallelism degree (FastFlow, poolEvolution)
    echo "--- Parallel version (FastFlow, poolEvolution) ---"
    for nw in ${nwlist[@]}
    do
        if [ "$nw" -eq 1 ]; then
            echo "Starting $runs runs, $nw worker."
        else
            echo "Starting $runs runs, $nw workers."
        fi
        for (( run=1; run<=$runs; run++))
        do
            a=$(./tsppar_ffpool $graph_file $pop_size $cross_prob $mut_prob $max_gen $nw $seed | tail -1 | cut -d ":" -f2 | cut -d " " -f2 | tr -d "\n")
            echo "Run $run, time: $a ms"
            echo "$nw $a" >> $dir/pool.csv
        done
    done
fi

echo ""

if [ ${exps[4]} -eq 1 ]; then
    # Start parallel runs for each parallelism degree (FastFlow, ParallelFor)
    echo "--- Parallel version (FastFlow, ParallelFor) ---"
    for nw in ${nwlist[@]}
    do
        if [ "$nw" -eq 1 ]; then
            echo "Starting $runs runs, $nw worker."
        else
            echo "Starting $runs runs, $nw workers."
        fi
        for (( run=1; run<=$runs; run++))
        do
            a=$(./tsppar_fffor $graph_file $pop_size $cross_prob $mut_prob $max_gen $nw $seed | tail -1 | cut -d ":" -f2 | cut -d " " -f2 | tr -d "\n")
            echo "Run $run, time: $a ms"
            echo "$nw $a" >> $dir/parfor.csv
        done
    done
fi


echo ""
echo "Experiment executed successfully."
