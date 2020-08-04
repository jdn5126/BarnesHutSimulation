CC=clang++
CFLAGS=-g --std=c++11

barnesHut: main.cpp Body.cpp OctTree.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I.

clean:
	rm -f barnesHut
