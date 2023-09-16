# genetic-tsp

Code for the Parallel and Distributed System course project (University of Pisa, MSc Artificial Intelligence, a.y. 2020-2021).

### Makefile
The project contains a Makefile to produce the executables and
the object files of the project. Running `make all` will be enough to get
everything ready. To replicate the same experiments and plots in the report, run `make exp`. If you need to, remember
to change the algorithm parameters at the start
of the script `experiment.sh` (default: experiment with 3000 individuals).


### Producing a graph file
Most of the executables need a `.csv` file containing the representation of the input graph. The file representing the graph used in the experiments
is already contained in the project directory (`complete.csv`).
To create a new complete graph with `n` nodes and random weights in `graph.csv`,
run:

    ./csv graph.csv n

### Testing the single versions
To test every genetic algorithm against the bruteforce algorithm, run the script
test.sh.
To test the sequential version, execute:

./tspseq complete.csv 1000 0.9 0.1 20 123

To run a parallel version, for example the ParallelFor one, run:

./tsppar_fffor complete.csv 1000 0.9 0.1 20 4 123


### Running the experiments
To replicate the complete experiment, run the bash script `experiment.sh` as follows:

    bash experiment.sh exp_dir 0,1,2,3,4 1 4 64 123

Again, if you need to, remember to change the algorithm parameters placed at the
start of the script.



### Plots
To produce plots, use the python script as follows:

    python3 data_analysis.py exp_dir

where `exp_dir` is the output directory of the script `experiment.sh`.
