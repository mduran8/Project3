//key.h
#include <sstream>
#include <iostream>
class key{
public:
key(std::string key);
std::string getKey(int i);
std::string ki[16];
private:
int r =0;
std::string per;
std::string subkey(std::string key);
};
