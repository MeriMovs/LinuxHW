all: prime-calculator

prime-calculator: prime-calculator.o
	g++ prime-calculator.o -o prime-calculator

prime-calculator.o: prime-calculator.cpp
	g++ -c prime-calculator.cpp -o prime-calculator.o

clean:
	rm -f prime-calculator.o prime-calculator