#ifndef RSA_H
#define RSA_H
#include "bigint.h"
#include <string>
using namespace std;
class RSA {
private:
    BigInt P;
    BigInt Q;
    BigInt N;
    BigInt publicKey;
    BigInt privateKey;

public:
    RSA();
    ~RSA();
    string getP();
    string getQ();
    string getN();
    string getPublicKey();
    string getPrivateKey();
    void setP(BigInt _P);
    void setQ(BigInt _Q);
    void setN(BigInt _N);
    void setPublicKey(BigInt _bk);
    void setPrivateKey(BigInt _vk);
    int generateKey(int keyLen);
    string encryption(string plainText);
    string decryption(string cipherText);
};
// TODO: 在此处引用程序需要的其他标头。

#endif // RSA_H
