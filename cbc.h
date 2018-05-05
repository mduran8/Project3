//cbc.h


#ifndef cbc_hpp
#define cbc_hpp

#include <stdio.h>
#include "Des.h"
#include <sstream>

class cbc {
private:
    
public:
  
    //Final resulting std::string
    std::stringstream mel;
    //math file object
    DESmath mathD;

    //Constructor
    cbc();
    
    //runCbc - runs the cbc algorithm
    std::string runCbc (std::string plainText, std::string IV, std::string K, std::string eORd);
    
    //Encrypt cbc
    std::string encryptCbc (std::string plainText, std::string IV, std::string K);
    
    //Decrypt cbc
    std::string decryptCbc (std::string cipher, std::string K, std::string IV);
    
    //XOR function
    std::string xored(std::string alpha, std::string beta);
    
};



#endif /* cbc_hpp */
