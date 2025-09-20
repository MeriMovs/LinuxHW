all: main

main: main.o complexNum.o merge.o 
	g++ main.o complexNum.o merge.o -o main

main.o: src/main.cpp include/complexNum.h include/merge.h
	g++ -c src/main.cpp -I./include -o main.o

complexNum.o: src/complexNum.cpp include/complexNum.h
	g++ -c src/complexNum.cpp -I./include -o complexNum.o

merge.o: src/merge.cpp include/merge.h include/complexNum.h
	g++ -c src/merge.cpp -I./include -o merge.o

run: main
	./main

clean:
	rm -f *.o main

.PHONY: all clean