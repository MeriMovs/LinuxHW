//Написать программу (C/C++), которая принимает как аргументы три числа a,b,c и печатает max и min: 

#include <iostream>
#include <algorithm>
#include <regex>

int main(int argc, char** argv){
	if(argc != 4){
		std::cerr << "Usage: " << argv[0] << " num1 num2 num3" << std::endl;
		return 1;
	}

	bool isAnum = std::regex_search( argv[1], std::regex( "\\D" )); 
	bool isBnum = std::regex_search( argv[2], std::regex( "\\D" )); 
	bool isCnum = std::regex_search( argv[3], std::regex( "\\D" ));
	
	if( isAnum || isBnum || isCnum ) {
		std::cerr << "Not a num" << std::endl;
        return 1;
	}
	int a = std::stoi(argv[1]);
    int b = std::stoi(argv[2]);
    int c = std::stoi(argv[3]);

    int min = std::min({a, b, c});
    int max = std::max({a, b, c});
    std::cout << "max: " << max << std::endl;
	std::cout << "min: " << min << std::endl;
	return 0;
}
