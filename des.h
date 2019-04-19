#ifndef DES_H
#define DES_H

#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

class Des {
	public:
		// Key-related sizes
		static const size_t INIT_KEY_SIZE = 64;
		static const size_t ACTUAL_KEY_SIZE = 56;
		static const size_t SUB_KEY_SIZE = 48;
		static const size_t HALF_KEY_SIZE = 28;
		static const size_t NUM_ROUNDS = 16;

		// Block-related sizes
		static const size_t BLOCK_SIZE = 64;
		static const size_t SBOX_ROW = 2;
		static const size_t SBOX_COL = 4;
		
		/*
		 * Performs the DES encryption or decryption; stores result in passed in bitsets
		 * Returns 0 
		 * @op: encrypt or decrypt
		 * @key: key to use for this operation
		 * @data: data to operate on
		 * @res: result of this operation
		 */
		static int des(const string op, const bitset<BLOCK_SIZE> key, const bitset<BLOCK_SIZE> data, bitset<BLOCK_SIZE>& res);

	private:
		/*
		 * Generates round keys from an initial key
		 * Returns vector containing 16 round keys
		 * @initKey: initial 64-bit key expand
		 */
		static vector<bitset<SUB_KEY_SIZE>> keygen(const bitset<INIT_KEY_SIZE> initkey);

		/*
		 * Perform initial permutation of message
		 * Splits message and permutes each half separately; stores result in passed in bitsets
		 * Returns 0
		 * @msg: block to permute
		 * @l0: permuted left half of msg
		 * @r0: permuted right half of msg
		 */
		static int initPerm(const bitset<BLOCK_SIZE> msg, bitset<BLOCK_SIZE/2>* l0, bitset<BLOCK_SIZE/2>* r0);

		/*
		 * DES round function
		 * Returns result of performing the round function on passed in Rn block
		 * @rn: block to perform function on
		 * @subkey: subkey for round function
		 */
		static bitset<BLOCK_SIZE/2> roundFunction(const bitset<BLOCK_SIZE/2> rn, const bitset<SUB_KEY_SIZE> subkey);
};

#endif /* DES_H */