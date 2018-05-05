//DESmath.h
#ifndef DESMATH_H
#define DESMATH_H
#include <sstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <string>

class DESmath{
public:
DESmath();
std::string sBoxintToBinary(int n);
std::string HexToBinary(std::string n);
std::string BinaryToHex(std::string n);
std::string HexToAscii(std::string n);
std::string AsciiToHex(std::string n);

};
#endif
