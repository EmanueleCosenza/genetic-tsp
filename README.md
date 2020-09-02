# genetic-tsp

Use 'make all' to build all the object files and executables of the project.
Use 'make exp' to execute the final experiment, testing all the versions and
    producing the final plots.
Use test.sh to test the genetic algorithms (against the exact brute force solution)

Every test is executed on a complete graph of 10 nodes.


Brief description of the executables:

tspseq: sequential version.
tsppar_th: C++ threads parallel version with active wait barriers.
tsppar_th_block: C++ threads parallel version with blocking barriers.
tsppar_ff: FastFlow parallel version.
tspbrute: sequential brute force algorithm to compute the exact solution of a TSP.
            Used to assess the quality of the approximated solution computed by the
            genetic algorithm.
csv: used to create a .csv file containing the representation of a complete weighted graph of
        a given number of nodes.
