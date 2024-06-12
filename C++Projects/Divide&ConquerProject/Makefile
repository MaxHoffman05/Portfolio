DEBUG = -g
CC = g++ -c $(DEBUG) -Wall -ansi -pedantic -std=c++20
LN = g++



runMe: main.o Object.o
	$(LN) -o runMe main.o Object.o

Object.o: Object.cpp Object.hpp
	$(CC) Object.cpp
main.o: main.cpp Object.hpp
	$(CC) main.cpp



