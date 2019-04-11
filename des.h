#ifndef DES_H
#define DES_H

#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

class Des {
	public:
		// Sizes
		static const size_t INIT_KEY_SIZE = 64;
		static const size_t SUB_KEY_SIZE = 48;
		static const size_t HALF_KEY_SIZE = 28;
		static const size_t NUM_ROUNDS = 16;

		// sizes
		static const size_t BLOCK_SIZE = 64;
		static const size_t SBOX_ROW = 2;
		static const size_t SBOX_COL = 4;
		
		static int test();
		static int des(const string op, const bitset<BLOCK_SIZE> key, const bitset<BLOCK_SIZE> data, bitset<BLOCK_SIZE>& res);

	private:
		// Key Generation
		static vector<bitset<SUB_KEY_SIZE>> keygen(const bitset<INIT_KEY_SIZE> initkey);
		// Initial Permutation
		static int initPerm(const bitset<BLOCK_SIZE> msg, bitset<BLOCK_SIZE/2>* l0, bitset<BLOCK_SIZE/2>* r0);
		// Round Function
		static bitset<BLOCK_SIZE/2> roundFunction(const bitset<BLOCK_SIZE/2> rn, const bitset<SUB_KEY_SIZE> subkey);
};

#endif /* DES_H */