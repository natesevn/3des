#ifndef KEYGEN_H
#define KEYGEN_H

#include <string>

using namespace std;

class Keygen {
	public:
		/* 
		 * Helper function to get cipher key based on parameters
		 * @pwd: password from which cipher key will be derived
		 * @key: buffer for resulting cipher key
		 */
		static int get3DesKey(char pwd[], unsigned char* key);

	private:
		/*
		 * Derive a key using PBKDF2 with specified parameters
		 * @pwd: character array from which key will be derived
		 * @salt: salt for PBKDF2
		 * @keysize: desired keysize
		 * @iter: desired number of iterations
		 * @key: buffer for resulting key
		 */
		static int getKey(char pwd[], unsigned char salt[], size_t keysize, int iter, unsigned char* key);
};

#endif /* KEYGEN_H */