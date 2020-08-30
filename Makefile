CXX = g++ -std=c++17

CXXFLAGS = -O3 -g -Wall -pedantic
OPTFLAGS = -finline-functions -DNDEBUG
INCLUDES = -I ./fastflow
LDFLAGS = -pthread

TARGETS = tspseq tsppar_sp tsppar_th tsppar_ff tspbrute csv
OBJS = tspseq.o tsppar_sp.o tsppar_th.o tsppar_ff.o tspbrute.o sync.o csv.o graph.o genetic.o

tspseq: tspseq.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@

tsppar_sp: tsppar_sp.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_th: tsppar_th.o sync.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_ff: tsppar_ff.o graph.o
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
