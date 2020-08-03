CC=clang++
CFLAGS=-g

barnesHut: main.cpp Body.cpp OctTree.cpp
	$(CC) $(CFLAGS) -o $@ $^ -I.

clean:
	rm -f barnesHut
