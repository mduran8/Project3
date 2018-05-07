/****main.cpp
debug = 1 to check input
debug = 2 to check decryption of session key
***/
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <utility>
#include <bitset>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include "DESmath.h"
#include "cbc.h"


/*
*/
using namespace std;

bool fexists(const std::string& filename);
void handleErrors(void);


int main(int argc, char* argv[]){
    
/******** Store variables given from terminal ********************/
    //inputs
    string typeCryption= argv[1]; //encryption or decryption
    string textFile = argv[2];//plain text message
    string sessionKey = argv [3];//session key
    string publicKey = argv [4];//public key
    string privateKey = argv [5];//private key
    string newSessionKey = "plainTextSessionKey.txt";
    string cipheredText = "cipheredText.txt";
    string signature = "signature.txt";

    //variables
    int debug = 2;
int decryptedtext_len, ciphertext_len;
    ifstream myfile (textFile,ios::binary);
    ifstream mySessionKey (sessionKey,ios::binary);
    ifstream myPublicKey (publicKey,ios::binary);
    ifstream myPrivateKey (privateKey,ios::binary);
    ofstream outSessionKey (newSessionKey,ios::binary);
    ofstream outCipheredText (cipheredText,ios::binary);
    ofstream outSignature (signature,ios::binary);


    //Verifying Input
    //Check for encryption/decryption flag
    if(!(typeCryption == "-e" ||typeCryption == "-d")){
	cerr<< "Please check the readme file for correct use program"<<endl<<"please choose encyrption(-e) or decyrption(-d)"<<endl;
	exit(1);
    }
    //Check textfile input is valid
    if(textFile.substr(textFile.size()-4,4) !=".txt"){
  	cerr<<"Please check filenames current names are invalid"<<endl;
	exit(1);
    }
     
    //Check input file exist
   if (fexists(textFile)== false) {
       cerr<<"Please check input file name. Current names does not exist."<<endl;
       	exit(1);
    }   
   //Check session key input 
    if (fexists(sessionKey)==false) {
	cerr<<"Please check session key file name. Current names does not exist."<<endl;
       	exit(1);
    }   
  
	//Get plain text
	string myString((std::istreambuf_iterator<char>(myfile)),
                 std::istreambuf_iterator<char>());
    	//Add padding if the given text is not appropriate
    	while(myString.size()%8!= 1) {myString.append("0");};
    	myfile.close();

	//Get sesion key
	string mySessionKeyString((std::istreambuf_iterator<char>(mySessionKey)),
                 		  std::istreambuf_iterator<char>());
       mySessionKey.close();
	//Get Public key
	string myPublicStringKey((std::istreambuf_iterator<char>(myPublicKey)),
       				 std::istreambuf_iterator<char>());
       myPublicKey.close();
	//Get Private Key
	string myPrivateStringKey((std::istreambuf_iterator<char>(myPrivateKey)),
       				 std::istreambuf_iterator<char>());
       myPrivateKey.close();

    if (debug==1){
	cout<<"decrypt/encrypt: "<<typeCryption<<endl;
	cout<<"Plain Text File name: "<<textFile<<endl;
	cout<<"Session Key File name: "<<sessionKey<<endl;
	cout<<"Public Key File name: "<<publicKey<<endl;
	cout<<"Private Key File name: "<<privateKey<<endl;
	cout<<"Plain Text Session Key File Name: "<<newSessionKey<<endl;
	cout<<"Ciphered Text File Name: "<<cipheredText<<endl;
	cout<<"Signature File Name: "<<signature<<endl;
	//What is on file
	cout<<"\nPlain Text: "<< myString<< endl;
	cout<<"\n Session Key Text: "<< mySessionKeyString<< endl;
	cout<<"\n Public Key Text: "<< myPublicStringKey<< endl;
	cout<<"\n Private Key Text: "<< myPrivateStringKey<< endl;

    }

/**********
Decrypt the session key
**********/
	EVP_PKEY_CTX *ctx;
	unsigned char *out, *in;
	size_t outlen, inlen;
	EVP_PKEY *key;

/*Set up the key*/
	FILE * EVPsessionKey_File = fopen ( sessionKey.c_str() , "r" );
	FILE * EVPPublicKey_File = fopen ( publicKey.c_str() , "r" );
	key = PEM_read_PUBKEY(EVPPublicKey_File, NULL, NULL, NULL);

/*Set up in, inlen*/
	fseek (EVPsessionKey_File , 0 , SEEK_SET);
	fseek (EVPsessionKey_File , 0 , SEEK_END);
	inlen = ftell (EVPsessionKey_File);
	in = (unsigned char*) malloc(inlen);
	fseek (EVPsessionKey_File , 0 , SEEK_SET);
	fread(in, 1, inlen, EVPsessionKey_File);

/*Set up the enviornment*/
	ctx = EVP_PKEY_CTX_new(key, NULL);
	if (!ctx){fputs ("EVP error: New CTX failed",stderr); exit (1);}
	if (EVP_PKEY_encrypt_init(ctx) <= 0) {fputs ("EVP error: Init CTX failed",stderr); exit (1);}
	if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_NO_PADDING) <= 0){fputs ("EVP error: Padding CTX failed",stderr); exit (1);}

/* Determine buffer length*/ 
	if (EVP_PKEY_encrypt(ctx, NULL, &outlen, in, inlen) <= 0)
		{fputs ("EVP error: Buffer length failed",stderr); exit (1);} 
	out = (unsigned char *)OPENSSL_malloc(outlen);
	if (!out) {fputs ("EVP error: Out not created",stderr); exit (1);}
	if (EVP_PKEY_encrypt(ctx, out, &outlen, in, inlen) <= 0)
	       {fputs ("EVP error: Encryption failed",stderr); exit (1);} 

/*Print the encrypted session key to a file*/
	if (debug == 1) cout<<"Decrypted session key: "<< out<< "\n" <<endl;
	ofstream dSessionKeyFile ("decryptedSesionKey.txt");
	dSessionKeyFile << out;

/*Close files */
	fclose(EVPsessionKey_File);
	fclose(EVPPublicKey_File);
/**********
Encrypt using DES
**********/
	
	stringstream append;
	for (std::size_t i = 0; i < 9; i++){
      		append << bitset<8>(out[i]);
  	}

	//Generate IV & Store it in file
	stringstream randomIV;
	ofstream outIV ("iv.txt",ios::binary);
    	char hexNums[] = {'1','2','3','4','5','6','7','8','9','A','B','C','D','F','E'};

 	for(int i = 0; i<16; i++)
        	randomIV << hexNums[rand()%16];
	string IV = randomIV.str();
 	outIV << IV;
	if (debug == 2) {
		cout<< "\n Session Key: "<< append.str()<<endl;
		cout<< "\n IV: " <<IV<<endl;
	}
/**********
Close everything that was opened
**********/

	//Close all the ofstream  
	outIV.close();
	outSessionKey.close();
	outCipheredText.close();
	outCipheredText.close();

    return 0;
}//end of main


//Checks if a file exists. If not returns false.
bool fexists(const string& filename) {
      ifstream ifile(filename.c_str());
      return (bool)ifile;
    }

//Handles errors that occr during EVP encryption
void handleErrors(void)
{
  //ERR_print_errors_fp(stderr);
   cout<<"Openssl error"<<endl;
  abort();
}


