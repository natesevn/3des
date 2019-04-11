/*string kappa = "hellodog";
	bitset<64> pog;
	for(char c : kappa) {
		pog = bitset<64>(c) | pog << 8;
	}
	cout << pog << endl;
	cout << pog.to_string() << endl;*/

#include <des.h>
#include <string>
#include <vector>
#include <bits/stdc++.h>

int main() {
	bitset<64> key(string("0001001100110100010101110111100110011011101111001101111111110001"));
	bitset<64> data(string("0000000100100011010001010110011110001001101010111100110111101111"));
	bitset<64> res;

	Des::des("encrypt", key, data, res);
	cout << res << endl;
	
	bitset<64> plain;
	Des::des("decrypt", key, res, plain);
	cout << plain << endl;
}