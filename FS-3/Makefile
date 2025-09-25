all: hacker-rm

hacker-rm: hacker-rm.o
	g++ hacker-rm.o -o hacker-rm

hacker-rm.o: hacker-rm.cpp
	g++ -c hacker-rm.cpp -o hacker-rm.o

clean:
	rm -f hacker-rm.o hacker-rm