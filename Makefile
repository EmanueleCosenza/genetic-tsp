CXX = g++

CXXFLAGS = -O3 -g -Wall -pedantic
LDFLAGS = -pthread

TARGETS = tspseq tspbrute csv
OBJS = tspseq.o tspbrute.o csv.o graph.o genetic.o

tspseq: tspseq.o graph.o genetic.o
	$(CXX) $(CXXFLAGS) $^ -o $@

tspbrute: tspbrute.o graph.o
	$(CXX) $(CXXFLAGS) $^ -o $@

csv: csv.o graph.o
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(TARGETS)

all: $(TARGETS)
