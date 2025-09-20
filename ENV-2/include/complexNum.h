#ifndef COMPLEX_NUMBER_H
#define COMPLEX_NUMBER_H

#include <iostream>

class ComplexNum {
private:
    double a, b;

public:
    ComplexNum(double r = 0.0, double i = 0.0);

    ComplexNum operator+(const ComplexNum& other) const;
    ComplexNum operator-(const ComplexNum& other) const;
    ComplexNum mul(double num) const;

    double absoluteValue() const;
    
    double getReal() const { return a; }
    double getImag() const { return b; }

    friend std::ostream& operator<<(std::ostream& os, const ComplexNum& c);
};

#endif