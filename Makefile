CXX = g++

CXXFLAGS = -O3 -I ../Utils
LDFLAGS = -pthread

OBJS = tspseq tspbrute csv

clean:
	rm -f $(OBJS) *~

all: $(OBJS)
