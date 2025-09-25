all: simple-copy

simple-copy: simple-copy.o
	g++ simple-copy.o -o simple-copy

simple-copy.o: simple-copy.cpp
	g++ -c simple-copy.cpp -o simple-copy.o

clean:
	rm -f simple-copy.o simple-copy