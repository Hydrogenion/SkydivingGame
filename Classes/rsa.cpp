#include <iostream>
#include "utils.h"
#include <ctime>
#include "rsa.h"

RSA::RSA() {
    srand((unsigned)time(NULL));
}

RSA::~RSA() {

}

string RSA::getN() {
    return to_string_hex(N);
}

string RSA::getP() {
    return to_string_hex(P);
}

string RSA::getQ() {
    return to_string_hex(Q);
}

string RSA::getPrivateKey() {
    return to_string_hex(privateKey);
}

string RSA::getPublicKey() {
    return to_string_hex(publicKey);
}

void RSA::setN(BigInt _N) {
    N = _N;
}

void RSA::setP(BigInt _P) {
    P = _P;
}

void RSA::setQ(BigInt _Q) {
    Q = _Q;
}

void RSA::setPrivateKey(BigInt _vk) {
    privateKey = _vk;
}

void RSA::setPublicKey(BigInt _bk) {
    publicKey = _bk;
}

int RSA::generateKey(int keyLen) {
    clock_t startTime, endTime;
    startTime = clock();
    P = generatePrime(keyLen/2);
    Q = generatePrime(keyLen/2);
    while (P == Q) Q = generatePrime(keyLen/2);
    N = P * Q;
    endTime = clock();
    BigInt fai = (P - BigInt("1")) * (Q - BigInt("1"));
    privateKey = randomBigInteger(rand()%keyLen+keyLen/2);
    while (!(gcd(privateKey, fai) == BigInt("1")))
        privateKey = privateKey + BigInt("2");
    publicKey = modReverse(privateKey, fai);
    return  (double)(endTime - startTime) / CLOCKS_PER_SEC * 1000;
}

string RSA::encryption(string plainText) {
    BigInt cipherText = power(BigInt(plainText,16), publicKey, N);
    return to_string_hex(cipherText);
}

string RSA::decryption(string cipherText) {
    BigInt plainText = power(BigInt(cipherText,16), privateKey, N);
    return to_string_hex(plainText);
}
