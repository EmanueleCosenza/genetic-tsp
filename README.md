# genetic-tsp

### Makefile
The project contains a Makefile to produce the executables and
the object files of the project. Just using make all will be enough to get
everything ready. Moreover, if you want to replicate the same exact experi-
ments and plots of this report, you can directly use make exp. Just remember
to change (if you need) the genetic algorithm parameters placed at the start
of the script experiment.sh (default: 3000 individuals experiment).


### Producing a graph file
Most of the executables need a .csv file contain-
ing the representation of the input graph. The graph used in the experiments
is already contained in the project directory as complete.csv.
To output a new complete graph with n nodes and random weights in graph.csv,
just execute:

./csv graph.csv n


### Testing the single versions
To test every genetic algorithm against the bruteforce algorithm, run the script
test.sh.
To test the sequential version, execute:

./tspseq complete.csv 1000 0.9 0.1 20 123

To run a parallel version, for example the ParallelFor one, run:

./tsppar_fffor complete.csv 1000 0.9 0.1 20 4 123


### Running the experiments
To replicate a complete experiment, just ex-
ecute the bash script experiment.sh in this way:

bash experiment.sh exp_dir 0,1,2,3,4 1 4 64 123

Just remember to change the genetic algorithm parameters placed at the
start of the script (default: 3000 individuals experiment).



### Plots
To produce plots, use the python script as follows:

python3 data_analysis.py exp_dir

where exp_dir is the output directory of the script experiment.sh
