#include <iostream>
#include <bits/stdc++.h>

using namespace std;

// Key Scheduling Constants
// Permutation 1
const unsigned int pc1[] = {57,   49,    41,   33,    25,    17,    9,
							1,   58,    50,   42,    34,    26,   18,
							10,    2,    59,   51,    43,    35,   27,
							19,   11,     3,   60,    52,    44,   36,
							63,   55,    47,   39,    31,    23,   15,
							7,   62,    54,   46,    38,    30,   22,
							14,    6,    61,   53,    45,    37,   29,
							21,   13,     5,   28,    20,    12,    4};
// Permutation 2
const unsigned int pc2[] = {14,    17,   11,    24,     1,    5,
							3,    28,   15,     6,    21,   10,
							23,    19,   12,     4,    26,    8,
							16,     7,   27,    20,    13,    2,
							41,    52,   31,    37,    47,   55,
							30,    40,   51,    45,    33,   48,
							44,    49,   39,    56,    34,   53,
							46,    42,   50,    36,    29,   32};
// Left-shifts
const unsigned int lShift[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
// Sizes
const size_t INIT_KEY_SIZE = 64;
const size_t SUB_KEY_SIZE = 48;
const size_t HALF_KEY_SIZE = 28;
const size_t NUM_ROUNDS = 16;

// Helper function since Bitset accesses elements from the right
constexpr int reverseNum(int size, int index) {
	return size - index - 1;
}

// Circular left shift 
template <size_t N>
bitset<N> rotate(bitset<N> b, unsigned m){
	b = b << m | b >> (N-m);
	return b;
}

// Concatenate 2 bitsets into 1; https://stackoverflow.com/questions/3061721/concatenate-boostdynamic-bitset-or-stdbitset
template <size_t N1, size_t N2 >
bitset <N1 + N2> concat( const bitset <N1> & b1, const bitset <N2> & b2 ) {
    string s1 = b1.to_string();
    string s2 = b2.to_string();
    return bitset <N1 + N2>( s1 + s2 );
}

// Generate 16 round keys
int keygen(bitset<INIT_KEY_SIZE> initkey) {
	bitset<SUB_KEY_SIZE> keyList[NUM_ROUNDS];

	bitset<HALF_KEY_SIZE> c0;
	bitset<HALF_KEY_SIZE> d0;
	int c = 0, d = 0;
	
	// Initial key permutation
	for(int i=0; i<HALF_KEY_SIZE*2; i++) {

		// Get left half i.e. c0
		if(i < HALF_KEY_SIZE) {
			// Add 1 because pc1 uses 1-indexing
			if(initkey.test(reverseNum(INIT_KEY_SIZE, pc1[i]) + 1)) {
				c0.set(reverseNum(HALF_KEY_SIZE, c));
			}
			c++;

		// Get right half i.e. d0	
		} else {
			if(initkey.test(reverseNum(INIT_KEY_SIZE, pc1[i]) + 1)) {
				d0.set(reverseNum(HALF_KEY_SIZE, d));
			}
			d++;
		}
	}

	bitset<HALF_KEY_SIZE*2> tempKey;
	bitset<SUB_KEY_SIZE> p2Key;
	int p = 0;

	// Creating 16 round keys
	for(int i=0; i<NUM_ROUNDS; i++) {
		c0 = rotate(c0, lShift[i]);
		d0 = rotate(d0, lShift[i]);

		tempKey = concat(c0, d0);
		
		// Permutation to get final 48-bit subkey
		p = 0;
		p2Key.reset();
		for(int i=0; i<SUB_KEY_SIZE; i++) {
			if(tempKey.test(reverseNum(HALF_KEY_SIZE*2, pc2[i]) + 1)) {
				p2Key.set(reverseNum(SUB_KEY_SIZE, p));
			}
			p++;
		}

		keyList[i] = p2Key;
	}
}


int main() {
	bitset<64> testkey(string("0001001100110100010101110111100110011011101111001101111111110001"));

	keygen(testkey);
	
}

//1111000011001100101010101111
//0101010101100110011110001111

/*
000110110000001011101111111111000111000001110010
011110011010111011011001110110111100100111100101
010101011111110010001010010000101100111110011001
011100101010110111010110110110110011010100011101
011111001110110000000111111010110101001110101000
011000111010010100111110010100000111101100101111
111011001000010010110111111101100001100010111100
111101111000101000111010110000010011101111111011
111000001101101111101011111011011110011110000001
101100011111001101000111101110100100011001001111
001000010101111111010011110111101101001110000110
011101010111000111110101100101000110011111101001
100101111100010111010001111110101011101001000001
010111110100001110110111111100101110011100111010
101111111001000110001101001111010011111100001010
110010110011110110001011000011100001011111110101
*/
