CXX = g++

CXXFLAGS = -O3 -g -Wall -pedantic
LDFLAGS = -pthread

TARGETS = tspseq tsppar_sp tsppar_th tspbrute csv
OBJS = tspseq.o tsppar_sp.o tsppar_th.o tspbrute.o sync.o csv.o graph.o genetic.o

tspseq: tspseq.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@

tsppar_sp: tsppar_sp.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

tsppar_th: tsppar_th.o sync.o graph.o genetic.o
		$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

sync.o: sync.cpp sync.hpp

tspbrute: tspbrute.o graph.o
	$(CXX) $(CXXFLAGS) $^ -o $@

csv: csv.o graph.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(TARGETS)

all: $(TARGETS)
