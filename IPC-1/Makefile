all: signal-echo

signal-echo: signal-echo.o
	g++ signal-echo.o -o signal-echo

signal-echo.o: signal-echo.cpp
	g++ -c signal-echo.cpp -o signal-echo.o

clean:
	rm -f signal-echo.o signal-echo