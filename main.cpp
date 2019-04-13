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

	bitset<64> key1(string("0011000100110010001100110011010000110101001101100011011100111000"));
	bitset<64> key2(string("0011100100110000001100010011001000110011001101000011010100110110"));
	bitset<64> key3(string("0100000101000010010000110100010001000101010001100100011101001000"));
	
	vector<bitset<Des::BLOCK_SIZE>> keyList;
	keyList.push_back(key1);
	keyList.push_back(key2);
	keyList.push_back(key3);

	// Encrypt => E1, D2, E3
	bitset<64> iv(string("0011000100110010001100110011010000110101001101100011011100111000"));
	vector<bitset<Des::BLOCK_SIZE>> cipherList = CBC_E(keyList, messageList, iv);

	stringstream test;
	for(auto it: cipherList) {
		test << hex << uppercase << it.to_ulong();
	}
	cout << test.str() << endl;
	
	// Decrypt => D3, E2, D1
	vector<string> resList = CBC_D(keyList, cipherList, iv);
	for(auto it : resList) {
		cout << it << endl;
	}
}