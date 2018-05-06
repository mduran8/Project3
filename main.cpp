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
    string IV = argv [2];
    string textFile = argv[3];//plain text message
    string sessionKey = argv [4];//session key
    string publicKey = argv [5];//public key
    string privateKey = argv [6];//private key
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
    //Check IV is valid
    if(IV.size() != 16){
	cerr<<"Please check IV incorrect hexidecimal size"<<endl;
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
    /*//Check public key input 
    if (fexists(publicKey)) {
	cerr<<"Please check public key file name. Current names does not exist."<<endl;
       	exit(1);
    }   
    //Check private key input 
    if (fexists(privateKey)) {
	cerr<<"Please check private key file name. Current names does not exist."<<endl;
       	exit(1);
    }   
*/

  
	//Get plain text
	string myString((std::istreambuf_iterator<char>(myfile)),
                 std::istreambuf_iterator<char>());
    	//Add padding if the given text is not appropriate
    	while(myString.size()%8!= 1) {myString+='0';}
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
	cout<<"IV: "<<IV<<endl;
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
unsigned char *out;
unsigned const char *in;
size_t outlen, inlen; 
EVP_PKEY *key;
FILE * EVPsessionKey_File = fopen ( sessionKey.c_str() , "r" );
FILE * EVPPublicKey_File = fopen ( publicKey.c_str() , "r" );

in = reinterpret_cast<unsigned const char *>(mySessionKeyString.c_str());

if (EVPPublicKey_File==NULL) {fputs ("File error: Public Key Could not be opened",stderr); exit (1);}
if (EVPsessionKey_File==NULL) {fputs ("File error: Session Key Could not be opened",stderr); exit (1);}

//Initialize enviornment
key = PEM_read_PUBKEY(EVPPublicKey_File, NULL, NULL, NULL);
ctx = EVP_PKEY_CTX_new(key,NULL);
 if (!ctx) {fputs ("EVP error: New CTX failed",stderr); exit (1);}
 if (EVP_PKEY_encrypt_init(ctx) <= 0) {fputs ("EVP error: Init CTX failed",stderr); exit (1);}
 if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_NO_PADDING) <= 0){fputs ("EVP error: Padding failed",stderr); exit (1);}


 /* Determine buffer length */
 if (EVP_PKEY_encrypt(ctx, NULL, &outlen, in, mySessionKeyString.size()-1) <= 0)

 out = (unsigned char *)OPENSSL_malloc(outlen);

 if (!out){fputs ("EVP error: Init CTX failed",stderr); exit (1);}
 
cout<< "I am here"<<endl;
 if (EVP_PKEY_encrypt(ctx, out, &outlen, in, mySessionKeyString.size()-1) <= 0){fputs ("EVP error: encryption failed",stderr); exit (1);}

 /* Encrypted data is outlen bytes written to buffer out */
/*

	EVP_PKEY_CTX* ctx;
	unsigned char *out;
	unsigned char *in; //in = session key, out = decrypted session key
	size_t outlength, inlength; //Length of in and our
	EVP_PKEY* EVPubKey;

	//Read ifstreams as array of char elements
	FILE * EVPsessionKey_File = fopen ( sessionKey.c_str() , "r" );
	FILE * EVPPublicKey_File = fopen ( publicKey.c_str() , "r" );


	/*Set up context*/
/*	if (EVPPublicKey_File==NULL) {fputs ("File error: Public Key Could not be opened",stderr); exit (1);}
	if (EVPsessionKey_File==NULL) {fputs ("File error: Session Key Could not be opened",stderr); exit (1);}

	// Get size of session key 
	fseek (EVPsessionKey_File , 0 , SEEK_END);
	inlength = ftell (EVPsessionKey_File);
	fseek (EVPsessionKey_File , 0 , SEEK_SET);
	in = (unsigned char*) malloc(inlength);
	fread(in, 1, inlength, EVPsessionKey_File);

	/* Determine buffer length*/ 
/*	EVPubKey = PEM_read_PUBKEY(EVPPublicKey_File, NULL, NULL, NULL);
	ctx = EVP_PKEY_CTX_new(EVPubKey,NULL);
	if (!ctx) {fputs ("CTX error",stderr); exit (1);}
	if (EVP_PKEY_encrypt_init(ctx) <= 0){fputs ("CTX error initializing encrypt",stderr); exit (1);}
	if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_NO_PADDING) <= 0){fputs ("CTX error with padding",stderr); exit (1);}


	

	/*Determine buffer length*/
 /*	if (EVP_PKEY_encrypt(ctx, NULL, &outlength, in, inlength) <= 0)
	out = (unsigned char *)OPENSSL_malloc(outlength);
	if (!out){fputs ("EVP error initializing out buffer",stderr); exit (1);}
	cout<< "I'm here"<<endl;
	/*Encrypt Session Key*/
/*	if (EVP_PKEY_encrypt(ctx, out, &outlength, in, inlength) <= 0){fputs ("EVP error encrypting session key ",stderr); exit (1);}
	
	cout<< "I'm here"<<endl;

        
	//Write it on to a file
	//ofstream dSessionKeyFile ("decryptedSesionKey.txt");
	//dSessionKeyFile << out;

	
/*


inlength = mySessionKeyString.size();
in = mySessionKeyString.c_str();


bio = BIO_new_file(publicKey.c_str(), "rb");
EVPubKey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
BIO_free_all(bio);

ctx =  EVP_PKEY_CTX_new(EVPubKey, NULL);
if (!ctx) cout<< "Error"<<endl;

if (EVP_PKEY_encrypt_init(ctx)<= 0) cout<< "Error"<<endl;

EVP_PKEY_CTX_set_rsa_padding(ctx,RSA_PKCS1_PSS_PADDING);

//Determine Buffer Length
if(EVP_PKEY_encrypt(ctx, NULL, &tempLength, in, inlength)<= 0) cout<< "Error"<<endl;

/*evpSessionKey.text = new unsigned char [tempLength];

EVP_PKEY_encrypt(ctx, NULL, &tempLength, (const unsigned char) mySessionKeyString.c_str() , mySessionKeyString.size());
result.length = tempLength;*/
/**********
Close everything that was opened
**********/

	//Close all the ofstream  
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


