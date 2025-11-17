all: demo-application

demo-application: demo-application.o lib_sch.so
	g++ demo-application.o -o demo-application -L. -l_sch -lpthread


demo-application.o: demo-application.cpp
	g++ -c demo-application.cpp -o demo-application.o

clean:
	rm -f *.o *.so *.out


lib_sch.so: lib_sch.cpp
	g++ -fPIC -c lib_sch.cpp -o lib_sch.o
	g++ -shared -o lib_sch.so lib_sch.o -lpthread

