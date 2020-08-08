CXX=/usr/local/opt/llvm/bin/clang++
LDFLAGS=-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib
CPPFLAGS=-I/usr/local/opt/llvm/include -g -std=c++11

barnesHut: main.cpp Body.cpp OctTree.cpp
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -fopenmp -I.

inputGen: inputGen.cpp Body.cpp
	$(CXX) $(CPPFLAGS) -o $@ $^ -I.

clean:
	rm -f barnesHut
	rm -f inputGen

all: barnesHut inputGen
