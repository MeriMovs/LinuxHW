#include "../include/complexNum.h"
#include <cmath>

ComplexNum::ComplexNum(double r, double i) : a(r), b(i) {}

ComplexNum ComplexNum::operator+(const ComplexNum& other) const {
    return ComplexNum(a + other.a, b + other.b);
}

ComplexNum ComplexNum::operator-(const ComplexNum& other) const {
    return ComplexNum(a - other.a, b - other.b);
}

ComplexNum ComplexNum::mul(double num) const {
    return ComplexNum(a * num, b * num);
}

double ComplexNum::absoluteValue() const {
    return std::sqrt(a * a + b * b);
}

std::ostream& operator<<(std::ostream& os, const ComplexNum& c) {
    if (c.b >= 0) {
        os << c.a << " + " << c.b << "i";
    } else {
        os << c.a << " - " << (-c.b) << "i";
    }

    return os;
}