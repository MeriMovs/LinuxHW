all: main

main: read.o
	g++ read.o -o main

read.o: read.cpp
	g++ -c read.cpp -o read.o

clean:
	rm -f read.o main