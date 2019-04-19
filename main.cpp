#include <des.h>
#include <keygen.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

const size_t KEY_SIZE = 24;

/* 
 * Converts string into binary string
 * Returns a bitset containing the representation of the string in bits
 * @in: the string to encode
 */
template <size_t N>
constexpr bitset<N> string2bin(string in) {
	bitset<N> out;
	for(char c : in) {
		out = bitset<N>(c) | out << 8;
	}
	return out;
}

/*
 * Converts an array of characters into bits
 * Returns a bitset containing the representation of the character array
 * @in: the character array to encode
 */
template <size_t N>
constexpr bitset<N> chararr2bin(unsigned char* in) {
	bitset<N> out;
	for(int i=0; i<strlen((char*)in)-1; i++) {
		out = bitset<N>(in[i]) | out << 8;
	}
	return out;
}

/*
 * Converts bitset into strings
 * Returns the string representation of a bitset
 * @in: the bitset to convert
 */
template <size_t N>
constexpr string bin2string(bitset<N> in) {
	string out;

	stringstream sstream(in.to_string());
    while(sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        out += c;
    }

	return out;
}

/*
 * Encryption with ECB chain mode
 * Returns a vector of ciphers
 * @keyList: list of 3 keys for 3DES
 * @messageList: list of messages to encrypt
 */
vector<bitset<Des::BLOCK_SIZE>> ECB_E(vector<bitset<Des::BLOCK_SIZE>> keyList, vector<bitset<Des::BLOCK_SIZE>> messageList) {
	vector<bitset<Des::BLOCK_SIZE>> cipherList;
	bitset<64> c1, c2, cipher;
	for(auto it: messageList) {
		Des::des("encrypt", keyList[0], it, c1);
		Des::des("decrypt", keyList[1], c1, c2);
		Des::des("encrypt", keyList[2], c2, cipher);
		cipherList.push_back(cipher);
	}	

	return cipherList;
}

/*
 * Decryption with ECB chain mode
 * Returns a vector of messages
 * @keyList: list of 3 keys for 3DES
 * @cipherList: list of ciphers to decrypt
 */
vector<string> ECB_D(vector<bitset<Des::BLOCK_SIZE>> keyList, vector<bitset<Des::BLOCK_SIZE>> cipherList) {
	vector<string> resList;
	bitset<64> r1, r2, res;
	for(auto it: cipherList) {
		Des::des("decrypt", keyList[2], it, r1);
		Des::des("encrypt", keyList[1], r1, r2);
		Des::des("decrypt", keyList[0], r2, res);
		string output = bin2string(res);
		resList.push_back(output);
	}

	return resList;
}

/*
 * Encryption with CBC chain mode
 * Returns a vector of ciphers
 * @keyList: list of 3 keys for 3DES
 * @messageList: list of messages to encrypt
 * @iv: iv used
 */
vector<bitset<Des::BLOCK_SIZE>> CBC_E(vector<bitset<Des::BLOCK_SIZE>> keyList, vector<bitset<Des::BLOCK_SIZE>> messageList, bitset<64> iv) {
	vector<bitset<Des::BLOCK_SIZE>> cipherList;
	bitset<64> c1, c2, cipher;
	bitset<64> temp;
	bool init = true;

	for(auto it: messageList) {

		// XOR feedback
		if(init) { 
			temp = it^iv;
			init = false;
		} else {
			temp = it^cipher;
		}

		Des::des("encrypt", keyList[0], temp, c1);
		Des::des("decrypt", keyList[1], c1, c2);
		Des::des("encrypt", keyList[2], c2, cipher);
		cipherList.push_back(cipher);
	}	

	return cipherList;
}

/*
 * Decryption with CBC chain mode
 * Returns a vector of messages
 * @keyList: list of 3 keys for 3DES
 * @cipherList: list of ciphers to decrypt
 * @iv: iv used
 */
vector<string> CBC_D(vector<bitset<Des::BLOCK_SIZE>> keyList, vector<bitset<Des::BLOCK_SIZE>> cipherList, bitset<64> iv) {
	vector<string> resList;
	bitset<64> r1, r2, res;
	bitset<64> temp, prevC;
	bool init = true;

	for(auto it: cipherList) {

		Des::des("decrypt", keyList[2], it, r1);
		Des::des("encrypt", keyList[1], r1, r2);
		Des::des("decrypt", keyList[0], r2, res);

		// XOR feedback
		if(init) { 
			temp = res^iv;
			init = false;
		} else {
			temp = res^prevC;
		}
		prevC = it;

		string output = bin2string(temp);
		resList.push_back(output);
	}

	return resList;
}

/*
 * Encryption with OFB chain mode
 * Returns a vector of ciphers
 * @keyList: list of 3 keys for 3DES
 * @messageList: list of messages to encrypt
 * @iv: iv used
 */
vector<bitset<Des::BLOCK_SIZE>> OFB_E(vector<bitset<Des::BLOCK_SIZE>> keyList, vector<bitset<Des::BLOCK_SIZE>> messageList, bitset<64> iv) {
	vector<bitset<Des::BLOCK_SIZE>> cipherList;
	bitset<64> s1, s2, randstream;
	bitset<64> cipher;
	bitset<64> temp;
	bool init = true;

	for(auto it: messageList) {

		// Encrypt IV first
		if(init) {
			Des::des("encrypt", keyList[0], iv, s1);
			Des::des("decrypt", keyList[1], s1, s2);
			Des::des("encrypt", keyList[2], s2, randstream);
			init = false;
		} else {
			Des::des("encrypt", keyList[0], temp, s1);
			Des::des("decrypt", keyList[1], s1, s2);
			Des::des("encrypt", keyList[2], s2, randstream);
		}

		cipher = randstream^it;
		temp = randstream;
		cipherList.push_back(cipher);
	}	

	return cipherList;
}

/*
 * Decryption with OFB chain mode
 * Returns a vector of messages
 * @keyList: list of 3 keys for 3DES
 * @cipherList: list of ciphers to decrypt
 * @iv: iv used
 */
vector<string> OFB_D(vector<bitset<Des::BLOCK_SIZE>> keyList, vector<bitset<Des::BLOCK_SIZE>> cipherList, bitset<64> iv) {
	vector<string> resList;
	bitset<64> s1, s2, randstream;
	bitset<64> temp;
	bool init = true;

	for(auto it: cipherList) {

		if(init) {
			Des::des("encrypt", keyList[0], iv, s1);
			Des::des("decrypt", keyList[1], s1, s2);
			Des::des("encrypt", keyList[2], s2, randstream);
			init = false;
		} else {
			Des::des("encrypt", keyList[0], temp, s1);
			Des::des("decrypt", keyList[1], s1, s2);
			Des::des("encrypt", keyList[2], s2, randstream);
		}

		string output = bin2string(randstream^it);
		temp = randstream;
		resList.push_back(output);
	}

	return resList;
}

/*
 * Derives a 192-bit key from an entered password using PBKDF2, and splits it into 3 64-bit keys for 3DES
 * Returns 0 on success, -1 otherwise
 * @pwd: entered user password
 * @keyList: vector to store resulting keys
 */
int getKeys(char *pwd, vector<bitset<Des::BLOCK_SIZE>> &keyList) {
	unsigned char *cipherKey = new unsigned char[KEY_SIZE];

	// Use PBKDF2 to get keys
	int status = Keygen::get3DesKey(pwd, cipherKey);
	if(status == -1) {
		return -1;
	}

	// Get key in binary
	bitset<192> key = chararr2bin<192>(cipherKey);

	// Split key into 3 parts
	bitset<192> mask(0xffffffffffffffff);
	bitset<64> key1((key & mask).to_ulong());
	key >>= 64;
	bitset<64> key2((key & mask).to_ulong());
	key >>= 64;
	bitset<64> key3((key & mask).to_ulong());
	
	// Create key vector
	keyList.push_back(key1);
	keyList.push_back(key2);
	keyList.push_back(key3);

	return 0;
}

/*
 * Splits user message into a vector of 64-bit binary strings
 * Returns a vector of bitsets
 * @message: the string to split and foramt
 */
vector<bitset<Des::BLOCK_SIZE>> formatMessage(string message) {
	string temp;
	int numPadB = 0;
	bitset<64> bitString;
	
	vector<bitset<Des::BLOCK_SIZE>> messageList;
	for(size_t i=0; i<message.size(); i+=8) {
		// Split and encode 8 characters at a time
		temp = message.substr(i, 8);
		bitString = string2bin<64>(temp);

		// Pad with whole bytes if necessary
		if(temp.length() < 8) {
			numPadB = 8-temp.length();
			
			for(int j=0; j<numPadB; j++) {
				bitString = bitString << 8 | bitset<64>(numPadB);
			}
		} 
		messageList.push_back(bitString);
	}

	return messageList;
}

/*
 * Encrypt the message given password and chain mode; stores resulting ciphers in passed in vector
 * Returns 0 on success, -1 on failure
 * @message: user message to encrypt
 * @password: password to convert to key
 * @choice: chain mode to use
 * @cipherList: vector to store resulting ciphers
 */
int encrypt(string message, string password, string choice, vector<bitset<Des::BLOCK_SIZE>> &cipherList) {
	int status = 0;

	// Derive 3DES keys from password
	char* pwd = &password[0];
	vector<bitset<Des::BLOCK_SIZE>> keyList;
	status = getKeys(pwd, keyList);
	if(status == -1) {
		cout << "Failed to get cipher key." << endl;
		return -1;
	}

	// Split messages into 64-bit blocks
	vector<bitset<Des::BLOCK_SIZE>> messageList;
	messageList = formatMessage(message);

	// Generate iv; convert it to 64-bit bitset
	unsigned char *temp_iv = new unsigned char[8];
	if (!RAND_bytes(temp_iv, 8)) {
		cout << "Error generating IV for encryption." << endl;
		return -1;
	}
	bitset<64> iv = chararr2bin<64>(temp_iv);

	// Encrypt based on chain mode
	if(choice == "ecb") {
		cipherList = ECB_E(keyList, messageList);
	} else if(choice == "cbc") {
		cipherList = CBC_E(keyList, messageList, iv);
	} else if(choice == "ofb") {
		cipherList = OFB_E(keyList, messageList, iv);
	} else {
		cout << "Invalid chain mode." << endl;
		return -1;
	}

	// Insert IV at the start of cipherlist
	auto it = cipherList.insert(cipherList.begin(), 1, iv);

	return 0;
}

/*
 * Decrypt the message given password and chain mode; stores resulting messages in passed in vector
 * Returns 0 on success, -1 on failure
 * @cipherList: ciphers to decrypt
 * @password: password to convert to key
 * @choice: chain mode to use
 * @resList: vector to store resulting messages
 */
int decrypt(vector<bitset<Des::BLOCK_SIZE>> cipherList, string password, string choice, vector<string> &resList) {
	int status = 0;

	// Derive 3DES keys from password
	char* pwd = &password[0];
	vector<bitset<Des::BLOCK_SIZE>> keyList;
	status = getKeys(pwd, keyList);
	if(status == -1) {
		cout << "Failed to get cipher key." << endl;
		exit(EXIT_FAILURE);
	}

	// Get iv; iv is first line read from file
	bitset<Des::BLOCK_SIZE> iv = cipherList.front();
	cipherList.erase(cipherList.begin());

	// Decrypt 
	if(choice == "ecb") {
		resList = ECB_D(keyList, cipherList);
	} else if(choice == "cbc") {
		resList = CBC_D(keyList, cipherList, iv);
	} else if(choice == "ofb") {
		resList = OFB_D(keyList, cipherList, iv);
	} else {
		cout << "invalid option" << endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}

int main() {
	int status = 0;

	string choice, result;
	cout << "Encrypt or decrypt: ";
	cin >> choice;
	cin.ignore();
	transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

	// Get password
	cout << "Enter password: ";
	string mypass;
	getline(cin, mypass);
	char* pwd = &mypass[0];

	if(choice == "encrypt")	 {

		cout << "Select ECB, CBC, or OFB: ";
		cin >> choice;
		cin.ignore();
		transform(choice.begin(), choice.end(), choice.begin(), ::tolower);

		string fileName;
		cout << "Enter filename: ";
		cin >> fileName;
		cin.ignore();

		string message;
		cout << "Enter message: ";
		getline(cin, message);

		vector<bitset<Des::BLOCK_SIZE>> cipherList;
		status = encrypt(message, mypass, choice, cipherList);
		if(status == -1) {
			exit(EXIT_FAILURE);
		}

		// Write output to binary file
		unsigned long n = 0;
		ofstream file_o(fileName, ios::binary);
		
		// Write iv + ciphers
		for (auto it : cipherList){
			n = it.to_ulong();
			file_o.write(reinterpret_cast<const char *>(&n), sizeof(n));
		}
		file_o.close();

	} else if(choice == "decrypt") {

		cout << "Select ECB, CBC, or OFB: ";
		cin >> choice;
		cin.ignore();
		transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
		
		string fileName;
		cout << "Enter filename: ";
		cin >> fileName;
		cin.ignore();

		// Get cipher from binary file
		unsigned long n = 0;
		vector<bitset<Des::BLOCK_SIZE>> cipherList;
		ifstream file_i (fileName, ios::binary);
		if(file_i.is_open()){
	
			while(true){
				file_i.read(reinterpret_cast<char *>(&n),sizeof(n));
				cipherList.push_back(bitset<64>(n));
	
				// Exit if EOF
				if(file_i.eof()) {
					break;
				}
			}

		} else {
			cout << "File not found" << endl;
			exit(EXIT_FAILURE);
		}
		file_i.close();

		// File IO always reads an extra element; get rid of it
		cipherList.pop_back();

		vector<string> resList;
		status = decrypt(cipherList, mypass, choice, resList);
		if(status == -1) {
			exit(EXIT_FAILURE);
		}

		cout << "Decrypted string: ";
		for(auto it : resList) {
			cout << it;
		}
		cout << endl;

	} else {
		cout << "invalid option" << endl;
		exit(EXIT_FAILURE);
	}

}