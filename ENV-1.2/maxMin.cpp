//Написать программу (C/C++), которая принимает как аргументы три числа a,b,c и печатает max и min: 

#include <iostream>
#include <algorithm>

int main(int argc, char** argv){
	int a = std::atoi(argv[1]);
    	int b = std::atoi(argv[2]);
    	int c = std::atoi(argv[3]);

    	int min = std::min({a, b, c});
    	int max = std::max({a, b, c});
    	std::cout << "max: " << max << std::endl;
	std::cout << "min: " << min << std::endl;
	return 0;
}
