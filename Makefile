CC=g++
CFLAGS=-g --std=c++11

inputGen: inputGen.cpp Body.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I.

barnesHut: main.cpp Body.cpp OctTree.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I.

clean:
	rm -f barnesHut
	rm -f inputGen


all: barnesHut inputGen
