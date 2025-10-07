all: clean main

main: main.o
	g++ main.o -o main

main.o: main.cpp
	g++ -c main.cpp -o main.o

clean:
	rm -f main.o main *.txt
# 	echo '' > result.txt