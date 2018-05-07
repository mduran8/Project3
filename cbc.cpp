//cbc.cpp
#include "cbc.h"
//Constructor
cbc::cbc (){}

//runCbc - runs the cbc algorithm
std::string cbc::runCbc (std::string plainText, std::string IV, std::string K, std::string eORd){
    std::string result;
    std::string binaryIv = mathD.HexToBinary(IV);

    if (eORd == "-e")
        result = encryptCbc(plainText, binaryIv, K);
    else if(eORd == "-d")
        result = decryptCbc(plainText, binaryIv, K);
    else{ 
	std::cerr<<"invalid cryption option"<<std::endl;
	exit(1);
   }  
      return result;
}

//Encrypt cbc
std::string cbc::encryptCbc (std::string plainText, std::string IV, std::string K){
	Des d;
	std::string substring = plainText.substr(0,64);
	std::string IK = IV;
	std::string result;
	std::stringstream append;
	int round = 0;
	while(round < (plainText.size()/64)){
		result = xored(IK,substring);
		IK = d.encryption(result,K);
		append << IK;
	  	round ++;
		substring = plainText.substr((round*64),64);
	}
	return append.str();
}

//Decrypt cbc
std::string cbc::decryptCbc (std::string cipher, std::string IV, std::string K ){
	
	Des d;
	std::string substring = cipher.substr(0,64);
	std::string IK = IV;
	std::string result;
	std::string result2;
	std::stringstream append;
	int round = 0;
	while(round < (cipher.size()/64)){
		result = d.decryption(substring,K);
		result2 = xored(IK,result);
		append << result2;
		IK = substring;
	  	round++;
		substring = cipher.substr((round*64),64);
	}
	return append.str();
	
}

std::string cbc::xored(std::string alpha, std::string beta){
std::stringstream xo;
for(int FU =0; FU< alpha.size(); FU++){
char a = alpha.at(FU);
char b = beta.at(FU);

if( (a == '0' && b == '0')||(a == '1' && b == '1')) xo<< "0";
else if( (a == '1' && b == '0')||(a == '0' && b == '1')) xo << "1";
else{
std::cerr << "conversion error"<< std::endl;
exit(1);
}//end of else statment
}//end of for loop (FU)
 return xo.str();
}// end of xored


