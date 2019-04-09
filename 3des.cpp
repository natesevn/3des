#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// Helper function since Bitset accesses elements from the right
int reverseNum(int size, int index) {
	return size - index - 1;
}

int keygen(bitset<64> initkey) {
	int pc1[] = {57,   49,    41,   33,    25,    17,    9,
				1,   58,    50,   42,    34,    26,   18,
				10,    2,    59,   51,    43,    35,   27,
				19,   11,     3,   60,    52,    44,   36,
				63,   55,    47,   39,    31,    23,   15,
				7,   62,    54,   46,    38,    30,   22,
				14,    6,    61,   53,    45,    37,   29,
				21,   13,     5,   28,    20,    12,    4};

	bitset<28> c0;
	bitset<28> d0;
	int c = 0, d = 0;
	
	for(int i=0; i<56; i++) {

		// Get left half i.e. c0
		if(i < 28) {
			// Add 1 because pc1 uses 1-indexing
			if(initkey.test(reverseNum(64, pc1[i]) + 1)) {
				c0.set(reverseNum(28, c));
			}
			c++;

		// Get right half i.e. d0	
		} else {
			if(initkey.test(reverseNum(64, pc1[i]) + 1)) {
				d0.set(reverseNum(28, d));
			}
			d++;
		}
	}

	cout << c0 << endl;
	cout << d0 << endl;
	
}


int main() {
	bitset<64> testkey(string("0001001100110100010101110111100110011011101111001101111111110001"));

	keygen(testkey);
	
}

//1111000011001100101010101111
//0101010101100110011110001111