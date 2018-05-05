//DES.h
#include <sstream>
#include <iostream>
#include "DESmath.h"

class Des{
public:
DESmath dMath;
Des();
std::string encryption(std::string D, std::string key);
std::string decryption(std::string D, std::string key);
private:
std::string intialP(std::string text);
std::string expand(std::string R);
std::string xored(std::string alpha, std::string beta);
std::string sboxes(std::string a);
std::string pboxes(std::string k2);
std::string finalP(std::string f16);

};
