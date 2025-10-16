all: interactive-shell

interactive-shell: interactive-shell.o
	g++ interactive-shell.o -o interactive-shell

interactive-shell.o: interactive-shell.cpp
	g++ -c interactive-shell.cpp -o interactive-shell.o

clean:
	rm -f interactive-shell.o interactive-shell