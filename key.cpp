//Key.cpp

#include "key.h"

const int PC1[56] = {
    57,   49,   41,   33,   25,   17,   9,
    1,    58,   50,   42,   34,   26,   18,
    10,   2,    59,   51,   43,   35,   27,
    19,   11,   3,    60,   52,   44,   36,
    63,   55,   47,   39,   31,   23,   15,
    7,    62,   54,   46,   38,   30,   22,
    14,   6,    61,   53,   45,   37,   29,
    21,   13,   5,    28,   20,   12,   4
};
// PC-2 table (permutation for generating each subkey)
const int PC2[48] = {
    14,   17,   11,   24,   1,    5,    3,    28,
    15,   6,    21,   10,   23,   19,   12,   4,
    26,   8,    16,   7,    27,   20,   13,   2,
    41,   52,   31,   37,   47,   55,   30,   40,
    51,   45,   33,   48,   44,   49,   39,   56,
    34,   53,   46,   42,   50,   36,   29,   32
};


key::key(std::string key){
	for(int j =0;j<16; j++){
		ki[j] = subkey(key);
	}

}

std::string key::subkey(std::string key){
std::stringstream k;
std::string subkeyl;
std::stringstream pc;
std::stringstream pc2;
std::string s;
int p =0;
while(p < 56 && r == 0){
pc << key.substr((PC1[p]-1),1);
p++;
}
if (r == 0)per = pc.str();
std::stringstream lk;
std::stringstream rk;
switch(r){
case 0: case 1: case 8: case 15: // offset by -1 so actually 1,2,9,16
  lk << per.substr(1,27) << per.substr(0,1); 
  rk << per.substr(29,27) << per.substr(28,1);
  break;
default:
  lk << per.substr(2,26) << per.substr(0,2); 
  rk << per.substr(30,26) << per.substr(28,2);
  break;  
}

 k << lk.str() << rk.str();
s = k.str();
per = k.str();
int y =0;
 while(y < 48){
pc2 << s.substr((PC2[y]-1),1);
y++;
}
 subkeyl = pc2.str();
r++;
 return subkeyl;

}
std::string key::getKey(int i){
   return ki[i];
}
