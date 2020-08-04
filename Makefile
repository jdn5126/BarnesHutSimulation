CC=g++
CFLAGS=-g

inputGen: inputGen.cpp Body.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I.

barnesHut: main.cpp Body.cpp OctTree.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I.

clean:
	rm -f barnesHut


all: barnesHut inputGen
