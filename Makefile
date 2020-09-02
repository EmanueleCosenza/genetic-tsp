CXX = g++ -std=c++17

CXXFLAGS = -O3 -g -Wall -pedantic
OPTFLAGS = -finline-functions -DNDEBUG
INCLUDES = -I ./fastflow
LDFLAGS = -pthread

TARGETS = tspseq tsppar_th tsppar_th_block tsppar_ff  tspbrute csv
OBJS = tspseq.o tsppar_th.o tsppar_ff.o tspbrute.o sync.o csv.o graph.o genetic.o

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

tsppar_th_block: tsppar_th.o sync.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) -DBLOCKING $^ -o $@ $(LDFLAGS)

tsppar_ff: tsppar_ff.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_ff.o: tsppar_ff.cpp
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
	bash experiment.sh complete.csv $(POP_SIZE) 0.9 0.1 $(MAX_GEN) $(RUNS) $(NW) 123
	python3 data_analysis.py $(RUNS) $(NW)
	rm test_data.csv
