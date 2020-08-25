CXX = g++

CXXFLAGS = -O3 -I ../Utils
LDFLAGS = -pthread

OBJS = completemap completemap1 simplemap aoverhead toverhead poverhead syscalls syscall-clone

clean:
	rm -f $(OBJS) *~

all: $(OBJS)
