#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <des.h>

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

// DES Constants
// Initial Permutation
const unsigned int initP[] = {58,    50,   42,    34,    26,   18,    10,    2,
							  60,    52,   44,    36,    28,   20,    12,    4,
							  62,    54,   46,    38,    30,   22,    14,    6,
							  64,    56,   48,    40,    32,   24,    16,    8,
							  57,    49,   41,    33,    25,   17,     9,    1,
							  59,    51,   43,    35,    27,   19,    11,    3,
							  61,    53,   45,    37,    29,   21,    13,    5,
							  63,    55,   47,    39,    31,   23,    15,    7};
// Final Permutation
const unsigned int finalP[] = {40,     8,   48,    16,    56,   24,    64,   32,
							   39,     7,   47,    15,    55,   23,    63,   31,
							   38,     6,   46,    14,    54,   22,    62,   30,
							   37,     5,   45,    13,    53,   21,    61,   29,
							   36,     4,   44,    12,    52,   20,    60,   28,
							   35,     3,   43,    11,    51,   19,    59,   27,
							   34,     2,   42,    10,    50,   18,    58,   26,
							   33,     1,   41,     9,    49,   17,    57,   25};
// Expansion
const unsigned int expR[] = {32,     1,    2,     3,     4,    5,
							  4,     5,    6,     7,     8,    9,
							  8,     9,   10,    11,    12,   13,
							 12,    13,   14,    15,    16,   17,
							 16,    17,   18,    19,    20,   21,
							 20,    21,   22,    23,    24,   25,
							 24,    25,   26,    27,    28,   29,
							 28,    29,   30,    31,    32,    1};
// S-boxes
const unsigned int S1[4][16] = 
{14,  4,  13,  1,   2, 15,  11,  8,   3, 10,   6, 12,   5,  9,   0,  7,
  0, 15,   7,  4,  14,  2,  13,  1,  10,  6,  12, 11,   9,  5,   3,  8,
  4,  1,  14,  8,  13,  6,   2, 11,  15, 12,   9,  7,   3, 10,   5,  0,
 15, 12,   8,  2,   4,  9,   1,  7,   5, 11,   3, 14,  10,  0,   6, 13};

const unsigned int S2[4][16] =
{15,  1,   8, 14,   6, 11,   3,  4,   9,  7,   2, 13,  12,  0,   5, 10,
  3, 13,   4,  7,  15,  2,   8, 14,  12,  0,   1, 10,   6,  9,  11,  5,
  0, 14,   7, 11,  10,  4,  13,  1,   5,  8,  12,  6,   9,  3,   2, 15,
 13,  8,  10,  1,   3, 15,   4,  2,  11,  6,   7, 12,   0,  5,  14,  9};

const unsigned int S3[4][16] =
{10,  0,   9, 14,   6,  3,  15,  5,   1, 13,  12,  7,  11,  4,   2,  8,
 13,  7,   0,  9,   3,  4,   6, 10,   2,  8,   5, 14,  12, 11,  15,  1,
 13,  6,   4,  9,   8, 15,   3,  0,  11,  1,   2, 12,   5, 10,  14,  7,
  1, 10,  13,  0,   6,  9,   8,  7,   4, 15,  14,  3,  11,  5,   2, 12};

const unsigned int S4[4][16] =
{7, 13,  14,  3,   0,  6,   9, 10,   1,  2,   8,  5,  11, 12,   4, 15,
13,  8,  11,  5,   6, 15,   0,  3,   4,  7,   2, 12,   1, 10,  14,  9,
10,  6,   9,  0,  12, 11,   7, 13,  15,  1,   3, 14,   5,  2,   8,  4,
 3, 15,   0,  6,  10,  1,  13,  8,   9,  4,   5, 11,  12,  7,   2, 14};

const unsigned int S5[4][16] =
{2, 12,   4,  1,   7, 10,  11,  6,   8,  5,   3, 15,  13,  0,  14,  9,
14, 11,   2, 12,   4,  7,  13,  1,   5,  0,  15, 10,   3,  9,   8,  6,
 4,  2,   1, 11,  10, 13,   7,  8,  15,  9,  12,  5,   6,  3,   0, 14,
11,  8,  12,  7,   1, 14,   2, 13,   6, 15,   0,  9,  10,  4,   5,  3};

const unsigned int S6[4][16] =
{12,  1,  10, 15,   9,  2,   6,  8,   0, 13,   3,  4,  14,  7,   5, 11,
 10, 15,   4,  2,   7, 12,   9,  5,   6,  1,  13, 14,   0, 11,   3,  8,
  9, 14,  15,  5,   2,  8,  12,  3,   7,  0,   4, 10,   1, 13,  11,  6,
  4,  3,   2, 12,   9,  5,  15, 10,  11, 14,   1,  7,   6,  0,   8, 13};

const unsigned int S7[4][16] =
{4, 11,   2, 14,  15,  0,   8, 13,   3, 12,   9,  7,   5, 10,   6,  1,
13,  0,  11,  7,   4,  9,   1, 10,  14,  3,   5, 12,   2, 15,   8,  6,
 1,  4,  11, 13,  12,  3,   7, 14,  10, 15,   6,  8,   0,  5,   9,  2,
 6, 11,  13,  8,   1,  4,  10,  7,   9,  5,   0, 15,  14,  2,   3, 12};

const unsigned int S8[4][16] =
{13,  2,   8,  4,   6, 15,  11,  1,  10,  9,   3, 14,   5,  0,  12,  7,
  1, 15,  13,  8,  10,  3,   7,  4,  12,  5,   6, 11,   0, 14,   9,  2,
  7, 11,   4,  1,   9, 12,  14,  2,   0,  6,  10, 13,  15,  3,   5,  8,
  2,  1,  14,  7,   4, 10,   8, 13,  15, 12,   9,  0,   3,  5,   6, 11};
// Round permutation
const unsigned int roundP[] = {16,   7,  20,  21,
							   29,  12,  28,  17,
							    1,  15,  23,  26,
							    5,  18,  31,  10,
							    2,   8,  24,  14,
							   32,  27,   3,   9,
							   19,  13,  30,   6,
							   22,  11,   4,  25};

/*
 * Reverses array index to access Bitset elements in order
 * Returns reversed index
 * @size: size of array
 * @index: index to reverse
 */
constexpr int reverseNum(int size, int index) {
	return size - index - 1;
}

/*
 * Circular left shift bitset
 * Returns shifted bitset
 * @b: bitset to shift
 * @m: number of circular shifts
 */
template <size_t N>
constexpr bitset<N> rotate(bitset<N> b, unsigned m){
	b = b << m | b >> (N-m);
	return b;
}

/*
 * Concatenates 2 bitsets into 1
 * Returns concatenated bitset
 * @b1: left half
 * @b2: right half
 * taken from: https://stackoverflow.com/questions/3061721/concatenate-boostdynamic-bitset-or-stdbitset
 */
template <size_t N1, size_t N2 >
constexpr bitset <N1 + N2> concat( const bitset <N1> & b1, const bitset <N2> & b2 ) {
    string s1 = b1.to_string();
    string s2 = b2.to_string();
    return bitset <N1 + N2>( s1 + s2 );
}

// Generate 16 round keys
vector<bitset<Des::SUB_KEY_SIZE>> Des::keygen(const bitset<Des::INIT_KEY_SIZE> initkey) {
	vector<bitset<Des::SUB_KEY_SIZE>> keyList;

	bitset<Des::HALF_KEY_SIZE> c0;
	bitset<Des::HALF_KEY_SIZE> d0;
	int c = 0, d = 0;
	
	// Initial key permutation
	for(int i=0; i<Des::HALF_KEY_SIZE*2; i++) {

		// Get left half of subkey i.e. c0
		if(i < Des::HALF_KEY_SIZE) {
			if(initkey.test(reverseNum(Des::INIT_KEY_SIZE, pc1[i]) + 1)) {
				c0.set(reverseNum(Des::HALF_KEY_SIZE, c));
			}
			c++;

		// Get right half of subkey i.e. d0	
		} else {
			if(initkey.test(reverseNum(Des::INIT_KEY_SIZE, pc1[i]) + 1)) {
				d0.set(reverseNum(Des::HALF_KEY_SIZE, d));
			}
			d++;
		}
	}

	bitset<Des::HALF_KEY_SIZE*2> tempKey;
	bitset<Des::SUB_KEY_SIZE> p2Key;
	int p = 0;

	// Create 16 round keys
	for(int i=0; i<Des::NUM_ROUNDS; i++) {

		// Circular left shift based on round number
		c0 = rotate(c0, lShift[i]);
		d0 = rotate(d0, lShift[i]);

		// Combine left and right halves
		tempKey = concat(c0, d0);
		
		// Permutation to get final 48-bit subkey
		p = 0;
		p2Key.reset();
		for(int i=0; i<Des::SUB_KEY_SIZE; i++) {
			if(tempKey.test(reverseNum(Des::HALF_KEY_SIZE*2, pc2[i]) + 1)) {
				p2Key.set(reverseNum(Des::SUB_KEY_SIZE, p));
			}
			p++;
		}

		keyList.push_back(p2Key);
	}

	return keyList;
}

// Initial Permutation
int Des::initPerm(const bitset<Des::BLOCK_SIZE> msg, bitset<Des::BLOCK_SIZE/2>* l0, bitset<Des::BLOCK_SIZE/2>* r0) {
	int l = 0, r = 0;

	for(int i=0; i<Des::BLOCK_SIZE; i++) {

		// Permute left half i.e. l0
		if(i < Des::BLOCK_SIZE/2) {
			if(msg.test(reverseNum(Des::BLOCK_SIZE, initP[i]) + 1)) {
				l0->set(reverseNum(Des::BLOCK_SIZE/2, l));
			}
			l++;

		// Permute right half i.e. r0	
		} else {
			if(msg.test(reverseNum(Des::BLOCK_SIZE, initP[i]) + 1)) {
				r0->set(reverseNum(Des::BLOCK_SIZE/2, r));
			}
			r++;
		}
	}

	return 0;
}

// Round Function
bitset<Des::BLOCK_SIZE/2> Des::roundFunction(const bitset<Des::BLOCK_SIZE/2> rn, const bitset<Des::SUB_KEY_SIZE> subkey) {

	// Expand rn
	bitset<Des::SUB_KEY_SIZE> eR;
	int r = 0;
	for(int i=0; i<Des::SUB_KEY_SIZE; i++) {
		if(rn.test(reverseNum(Des::BLOCK_SIZE/2, expR[i]) + 1)) {
			eR.set(reverseNum(Des::SUB_KEY_SIZE, r));
		}
		r++;
	}

	// Expanded rn XOR subkey
	bitset<Des::SUB_KEY_SIZE> xR;
	xR = eR ^ subkey;

	// Using S-box for substitution
	bitset<Des::BLOCK_SIZE/2> sR;
	bitset<4> temp;
	bitset<Des::SBOX_ROW> row;
	bitset<Des::SBOX_COL> col;

	int i=0, j=6, s=0, sboxNum=0, sboxVal;

	while(j <= Des::SUB_KEY_SIZE) {

		// Get groups of 6-bits for S-box row, col
		row.reset();
		col.reset();
		while(i < j) {
			if((j-i-1) == 5) {
				row[1] = xR[reverseNum(Des::SUB_KEY_SIZE, i)];
			} else if((j-i-1) == 0) {
				row[0] = xR[reverseNum(Des::SUB_KEY_SIZE, i)];
			} else {
				col[j-i-2] = xR[reverseNum(Des::SUB_KEY_SIZE, i)];
			}
			i++;
		}
		j+=6;

		// Substitute each 6-bit block using the S-box
		switch(sboxNum) {
			case 0:
				sboxVal = S1[row.to_ulong()][col.to_ulong()];
				break;
			case 1:
				sboxVal = S2[row.to_ulong()][col.to_ulong()];
				break;
			case 2:
				sboxVal = S3[row.to_ulong()][col.to_ulong()];
				break;
			case 3: 
				sboxVal = S4[row.to_ulong()][col.to_ulong()];
				break;
			case 4:
				sboxVal = S5[row.to_ulong()][col.to_ulong()];
				break;
			case 5:
				sboxVal = S6[row.to_ulong()][col.to_ulong()];
				break;
			case 6:
				sboxVal = S7[row.to_ulong()][col.to_ulong()];
				break;
			case 7:
				sboxVal = S8[row.to_ulong()][col.to_ulong()];
				break;
			default: 
				cout << "FAILED" << endl;
		}
		sboxNum++;
		temp = bitset<4>(sboxVal);
		sR = bitset<Des::BLOCK_SIZE/2>(temp.to_ulong()) | sR << 4;
	}

	// Round permutation
	bitset<Des::BLOCK_SIZE/2> pR;
	int p=0;
	for(int i=0; i<Des::BLOCK_SIZE/2; i++) {
		if(sR.test(reverseNum(Des::BLOCK_SIZE/2, roundP[i]) + 1)) {
			pR.set(reverseNum(Des::BLOCK_SIZE/2, p));
		}
		p++;
	}	

	return pR;
}

int Des::des(const string op, const bitset<Des::BLOCK_SIZE> key, const bitset<Des::BLOCK_SIZE> data, bitset<Des::BLOCK_SIZE>& res) {
	// Get subkeys
	vector<bitset<Des::SUB_KEY_SIZE>> keyList = keygen(key);
	if(op.compare("decrypt") == 0) {reverse(keyList.begin(),keyList.end());}

	// DES Operations
	bitset<32> left;
	bitset<32> right;
	bitset<32> newLeft;
	bitset<32> newRight;

	// Initial perm
	initPerm(data, &left, &right);

	// Round 1
	newLeft = right;
	newRight = roundFunction(right, keyList[0]) ^ left;
	// Rounds 2-16
	for(int i=1; i<16; i++) {
		left = newLeft;
		right = newRight;

		newLeft = right;
		newRight = roundFunction(right, keyList[i]) ^ left;
	}

	// Final perm
	bitset<64> preFP = concat(newRight, newLeft);
	bitset<64> finalBlock;
	int f=0;
	for(int i=0; i<Des::BLOCK_SIZE; i++) {
		if(preFP.test(reverseNum(Des::BLOCK_SIZE, finalP[i]) + 1)) {
			finalBlock.set(reverseNum(Des::BLOCK_SIZE, f));
		}
		f++;
	}
	res = finalBlock;

	return 0;
}