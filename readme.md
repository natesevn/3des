# 3DES Implementation in C++
Run `run.sh` for a quick demo. Note that the program saves the encryption output in a binary file.

To use:
1. Requires `openssl 1.1.1.b-1` for key derivation.
2. Compile using the following command: `g++ keygen.cpp des.cpp main.cpp -I./ -g -lcrypto`
3. Run and follow instructions.
4. Output of encryption is stored in specified filename.
5. Input for decryption is read from specified filename.
6. If wrong password is supplied, decrypted string will be garbage.