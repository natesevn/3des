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

int main() {
	cout << "type 8 char msg: ";
	string message;
	getline(cin, message);
	bitset<64> plain = string2bin<64>(message);

	//cout << "type 8 char password: ";
	//string pw;
	//cin >> pw;
	//bitset<64> key = string2bin<64>(pw);

	bitset<64> key1(string("100100011010001010110011110001001101010111100110111101111"));
	bitset<64> key2(string("10001101000101011001111000100110101011110011011110111100000001"));
	bitset<64> key3(string("100010101100111100010011010101111001101111011110000000100100011"));

	// Encrypt => E1, D2, E3
	bitset<64> c1, c2, cipher;
	Des::des("encrypt", key1, plain, c1);
	Des::des("decrypt", key2, c1, c2);
	Des::des("encrypt", key3, c2, cipher);
	cout << "bin cipher is: " << cipher.to_string() << endl;
	
	// Decrypt => D3, E2, D1
	bitset<64> r1, r2, res;
	Des::des("decrypt", key3, cipher, r1);
	Des::des("encrypt", key2, r1, r2);
	Des::des("decrypt", key1, r2, res);
	string output = bin2string(res);
	cout << "decrypted cipher is: " << output << endl;
}