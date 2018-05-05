//DESmath.cpp

#include "DESmath.h"
//constructor
DESmath::DESmath(){};



//conversion function used from the sbox to give a 4 digit
std::string DESmath::sBoxintToBinary(int n){

switch(n){
	   case 0:
            return "0000"; 
	    break;
            case 1: 
	    return"0001";
	    break;
            case 2: 
            return "0010"; 
	    break;
            case 3: 
	    return "0011";
	    break;
            case 4: 
	    return "0100";
	    break;	
            case 5:
	    return "0101";
	    break;
            case 6: 
	    return "0110";
	    break;
            case 7: 
	    return "0111";
            break;
            case 8: 
	    return "1000";
	    break;
            case 9:
	    return "1001"; 
            break;
            case 10:
            return "1010"; 
            break;
            case 11: 
	    return "1011"; 
            break;
            case 12:
	    return "1100"; 
            break;
            case 13: 
            return "1101"; 
            break;
            case 14: 
	    return "1110"; 
            break;
            case 15:
	    return "1111"; 
            break;
        }
}//end sBoxintToBinary

std::string DESmath::HexToBinary(std::string n){
int i=0;
std::stringstream val;

 while( i<n.size()){
        switch(std::toupper(n[i])){
            case '0':
            val << "0000"; 
	    break;
            case '1': 
	    val <<"0001";
	    break;
            case '2': 
            val <<"0010"; 
	    break;
            case '3': 
	    val <<"0011";
	    break;
            case '4': 
	    val <<"0100";
	    break;	
            case '5':
	    val <<"0101";
	    break;
            case '6': 
	    val << "0110";
	    break;
            case '7': 
	    val << "0111";
            break;
            case '8': 
	    val <<"1000";
	    break;
            case '9':
	    val <<"1001"; 
            break;
            case 'A':
            val <<"1010"; 
            break;
            case 'B': 
	    val <<"1011"; 
            break;
            case 'C':
	    val <<"1100"; 
            break;
            case 'D': 
            val <<"1101"; 
            break;
            case 'E': 
	    val <<"1110"; 
            break;
            case 'F':
	    val << "1111"; 
            break;
        }
 	i++;
    }
   return val.str();
}//end of HexToBinary
std::string DESmath::BinaryToHex(std::string n){
int i=0;
std::stringstream val;
std::string k;
 while( i<n.size()){
        k = n.substr(i,4);
        
            if (k=="0000")val << '0'; 
	    else if(k=="0001") val <<'1';
	    else if(k=="0010")val <<'2'; 
	    else if(k=="0011")val <<'3';
	    else if(k=="0100")val <<'4';
	    else if(k=="0101")val <<'5';
	    else if(k=="0110")val << '6';
	    else if(k=="0111")val << '7';
            else if(k=="1000")val <<'8';
	    else if(k=="1001")val <<'9'; 
            else if(k=="1010")val <<'A'; 
            else if(k=="1011")val <<'B'; 
            else if(k=="1100")val <<'C'; 
            else if(k=="1101")val <<'D'; 
            else if(k=="1110")val <<'E'; 
            else if(k=="1111")val << 'F';
            else {
		std::cerr <<"invalid binary to hex conversion";
		exit(1);
	    } 
        
 	i= i+4;
    }
   return val.str();
}//end of BinaryToHex;

std::string DESmath::HexToAscii(std::string n){

std::stringstream newstring;
for(int i=0; i< n.size(); i+=2)
{
    std::string byte = n.substr(i,2);
    char chr = (char) (int)strtol(byte.c_str(), NULL, 16);
    newstring <<chr;
}
     return newstring.str();
}
std::string DESmath::AsciiToHex(std::string n){
std::string ofResults;
std::stringstream result;
// to string of hex
   result <<std::hex << std::uppercase;
   std::copy(n.begin(), n.end(), std::ostream_iterator<unsigned int>(result));  
   ofResults = result.str();
  return ofResults;

}


