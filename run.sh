g++ keygen.cpp des.cpp main.cpp -I./ -g -lcrypto -o 3des 

echo "Starting code. Enter encrypt to begin encrypting. Make sure you remember the password, filename, and mode used."
./3des

echo " "
echo "Starting code. Enter decrypt to begin decrypting previous file. Make sure you enter the correct password, filename, and mode used."
./3des
