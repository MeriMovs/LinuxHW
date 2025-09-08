//Написать программу (C/C++), которая будет печатать все значения аргументов командной строки в обратном порядке

#include <iostream>

int main(int argc, char** argv) {
	for (int i = argc-1; i > 0; --i) {
		std::cout << argv[i] << " "; 
	} std::cout << std::endl;
	return 0;
	
}

