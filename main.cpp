#include <des.h>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

template <size_t N>
constexpr bitset<N> string2bin(string in) {
	bitset<N> out;
	for(char c : in) {
		out = bitset<N>(c) | out << 8;
	}
	return out;
}

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

int main() {
	cout << "type 8 char msg: ";
	string message;
	getline(cin, message);

	// Split message into 64-bit block
	vector<bitset<Des::BLOCK_SIZE>> messageList;
	string temp;
	int numPadB = 0;
	bitset<64> bitString;
	
	for(size_t i=0; i<message.size(); i+=8) {
		temp = message.substr(i, 8);
		bitString = string2bin<64>(temp);

		// Pad if necessary
		if(temp.length() < 8) {
			numPadB = 8-temp.length();
			
			for(int j=0; j<numPadB; j++) {
				bitString = bitString << 8 | bitset<64>(numPadB);
			}
		} 
		messageList.push_back(bitString);
	}

	//cout << "type 8 char password: ";
	//string pw;
	//cin >> pw;
	//bitset<64> key = string2bin<64>(pw);

	bitset<64> key1(string("100100011010001010110011110001001101010111100110111101111"));
	bitset<64> key2(string("10001101000101011001111000100110101011110011011110111100000001"));
	bitset<64> key3(string("100010101100111100010011010101111001101111011110000000100100011"));
	
	vector<bitset<Des::BLOCK_SIZE>> keyList;
	keyList.push_back(key1);
	keyList.push_back(key2);
	keyList.push_back(key3);

	// ECB
	// Encrypt => E1, D2, E3
	vector<bitset<Des::BLOCK_SIZE>> cipherList = ECB_E(keyList, messageList);

	// Decrypt => D3, E2, D1
	vector<string> resList = ECB_D(keyList, cipherList);
	for(auto it : resList) {
		cout << it << endl;
	}
}