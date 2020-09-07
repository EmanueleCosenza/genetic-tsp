CXX = g++ -std=c++17

CXXFLAGS = -O3 -g -Wall -pedantic
OPTFLAGS = -finline-functions -DNDEBUG
INCLUDES = -I ./fastflow
LDFLAGS = -pthread

TARGETS = tspseq tsppar_th tsppar_th_block tsppar_ff tsppar_ffpool tsppar_fffor tspbrute csv
OBJS = tspseq.o tsppar_th.o tsppar_th_block.o tsppar_ff.o tsppar_ffpool.o tsppar_fffor.o tspbrute.o sync.o csv.o graph.o genetic.o

POP_SIZE = 1000
MAX_GEN = 100
RUNS = 8
NW = 64


.PHONY: all clean exp
.SUFFIXES: .cpp .o .hpp

tspseq: tspseq.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@

tsppar_th: tsppar_th.o sync.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_th_block: tsppar_th_block.o sync.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_th_block.o: tsppar_th.cpp
	$(CXX) -c -DBLOCKING $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_ff: tsppar_ff.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_ff.o: tsppar_ff.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_ffpool: tsppar_ffpool.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_ffpool.o: tsppar_ffpool.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_fffor: tsppar_fffor.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_fffor.o: tsppar_fffor.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) $^ -o $@ $(LDFLAGS)

sync.o: sync.cpp sync.hpp

tspbrute: tspbrute.o graph.o
	$(CXX) $(CXXFLAGS) $^ -o $@

csv: csv.o graph.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(TARGETS)

all: $(TARGETS)

exp: $(TARGETS)
	bash experiment.sh exp 0,1,2,3,4 1 4 64 123
	python3 data_analysis.py exp
