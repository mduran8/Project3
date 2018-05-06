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
#include "DESmath.h"
#include "cbc.h"


/*
*/

struct evpBuf {
	int size;
	unsigned char* text;
};

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
evpBuf evpSessionKey;
size_t tempLength, outLength;
EVP_PKEY* EVPubKey;
EVP_PKEY_CTX* ctx;
BIO* bio;


//Copy the session key into a const char array
int n =mySessionKeyString.length()+1;
unsigned char *out;
unsigned char *in;
size_t inlength;

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


