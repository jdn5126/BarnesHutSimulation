IDIR=./include
CXX=g++
#CXX=/usr/local/opt/llvm/bin/clang++
LDFLAGS=-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib
#CPPFLAGS=-I/usr/local/opt/llvm/include -I$(IDIR) -std=c++11 -fopenmp
CPPFLAGS=-I$(IDIR) -std=c++11 -fopenmp

all: barnesHutParallel barnesHut bruteForce inputGen

barnesHutParallel: ./src/barnesHutParallel.cpp ./src/OctTree.cpp ./src/Node.cpp ./src/Body.cpp ./src/Timer.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -Wall -Werror -O2

barnesHut: ./src/barnesHut.cpp ./src/OctTree.cpp ./src/Node.cpp ./src/Body.cpp ./src/Timer.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -Wall -Werror -O2

bruteForce: ./src/bruteForce.cpp ./src/Node.cpp ./src/Body.cpp ./src/Timer.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -Wall -Werror -O2

inputGen: ./src/inputGen.cpp ./src/Body.cpp ./src/Timer.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -Wall -Werror -O2

clean:
	rm -f barnesHutParallel
	rm -f barnesHut
	rm -f bruteForce
	rm -f inputGen
