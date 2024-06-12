
DBG=-g
OPTS=$(DBG) $(OPTIM) -Wall -ansi -pedantic -std=c++20

CC=g++ $(OPTS)  -c
LNK=g++

OBJS=project1.o Heap.o
GLBL=Makefile Heap.hpp

proj1: $(OBJS)
	$(LNK) -o proj1 $(OBJS)

project1.o: project1.cpp Heap.hpp $(GLBL)
	$(CC) project1.cpp

Heap.o: Heap.cpp Heap.hpp $(GLBL)
	$(CC) Heap.cpp

clean:
	/bin/rm -f *.o tokenify *~
