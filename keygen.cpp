#include <keygen.h>
#include <string>
#include <cstring>
#include <openssl/evp.h>
#include <iostream>

#define ITERATION 100000

using namespace std;

int Keygen::getKey(char pwd[], unsigned char salt[], size_t keysize, int iter, unsigned char* key) {
	size_t i;
	// Buffer to hold resulting key
	unsigned char *out = new unsigned char[keysize];

	// Object holding hash algorithm
	const EVP_MD* sha = EVP_sha512();
	
	// Get key using PBKDF2
	int status = 0;
	status = PKCS5_PBKDF2_HMAC(pwd, strlen(pwd), salt, strlen((char*)salt), iter, sha, keysize, out);
    if( status != 0 )
    {	
		memcpy(key, out, keysize);
		delete[] out;
		return 0;
    }
    else
    {
		delete[] out;
		return -1;
    }

	return status;
}

int Keygen::get3DesKey(char pwd[], unsigned char* key) {
	// Buffer to hold the master key and salt
    unsigned char *out = new unsigned char[24]; 
    unsigned char salt_value[] = {'c','i','p','h','e','r'};

	int result = 0;
	// Get cipher key
	result = getKey(pwd, salt_value, 24, ITERATION, out);

	// Copy cipher key to passed in buffer
	memcpy(key, out, 24);
	delete[] out;

	return result;
}

