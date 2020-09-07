#!/bin/bash

file=complete.csv

echo "Testing the sequential and parallel genetic algorithms against the exact bruteforce solution..."
echo ""

sleep 3
echo "Starting the computation of the exact bruteforce solution..."
sleep 3

./tspbrute complete.csv
echo ""

sleep 3
echo "Starting the execution of the sequential genetic algorithm..."
sleep 3

./tspseq complete.csv 1000 0.9 0.1 100 123
echo ""

sleep 3
echo "Starting the execution of the parallel genetic algorithm (C++ threads, spin barrier) with 4 workers..."
sleep 3

./tsppar_th complete.csv 1000 0.9 0.1 100 4 123
echo ""

sleep 3
echo "Starting the execution of the parallel genetic algorithm (C++ threads, blocking barrier) with 4 workers..."
sleep 3

./tsppar_th_block complete.csv 1000 0.9 0.1 100 4 123
echo ""

sleep 3
echo "Starting the execution of the parallel genetic algorithm (FastFlow, ParallelFor) with 4 workers..."
sleep 3

./tsppar_fffor complete.csv 1000 0.9 0.1 100 4 123
echo ""

sleep 3
echo "Starting the execution of the parallel genetic algorithm (FastFlow, poolEvolution) with 4 workers..."
sleep 3

./tsppar_ffpool complete.csv 1000 0.9 0.1 100 4 123
echo ""

sleep 3
echo "Exiting."
