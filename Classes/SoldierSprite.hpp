//
//  SoldierSprite.hpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/13.
//

#ifndef SoldierSprite_hpp
#define SoldierSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "bigint.h"

using namespace cocos2d;

class SoldierSprite : public Sprite{
public:
    SoldierSprite();
    static SoldierSprite* create(const std::string& filename, const Rect& rect);
    void setSpeed(int _speed) {speed = _speed;}
    void setPrivateKeyD(std::string _privateKeyD) {privateKeyD = _privateKeyD;}
    void setPublicKeyN(std::string _publicKeyN) {publicKeyN = _publicKeyN;}
    void setPublicKeyE(std::string _publicKeyE) { publicKeyE = _publicKeyE; }
    void setPublicKeysN(std::vector<std::string> _publicKeysN) {publicKeysN = _publicKeysN;}
    void setPublicKeysE(std::vector<std::string> _publicKeysE) { publicKeysE = _publicKeysE; }
    void setId(int _id) { id = _id; _index = id-'a'; }
    void setShareKey(double _shareKey) {shareKey = _shareKey;}
    void setRank(int rank) {_rank = rank;}
    void setMaxRank(int max_rank) {_max_rank = max_rank;}
    void setTotal(int total) {_total = total;}
    void setSoldierGroup(Vector<SoldierSprite*> *ptr){soldiers = ptr;}
    
    int getSpeed(){return speed;}
    int getRank(){return _rank;}
    std:: string getRankS() {return std::to_string(rank);}
    std::string getPrivateKeyD() {return privateKeyD; }
    std::string getPublicKeyN() {return publicKeyN; }
    std::string getPublicKeyE() { return publicKeyE; }
    double getShareKey() {return shareKey;}
    int getID(){return id;}
    Label* info;
    
    std::string rsaEncryption(std::string plainText, int id);
    std::string rsaDecryption(std::string ciperText);
    
    BigInt rsaEncryptionBigInt(BigInt plainText, int id);
    BigInt rsaDecryptionBigInt(BigInt ciperText);

    
    std::string sendIdentification();
    bool recieveIdentification(std::string ciperText, int id);
    
    void messageProcess(std::string msg,Vec2 Pos);
    
    
    int compare_once();
    bool require_comparison();
    //millionaire
    static int compare(SoldierSprite& A, SoldierSprite& B, int index_A, int index_B);
    BigInt step1(int to_index);
    std::vector<BigInt> step2(BigInt);
    bool step3(std::vector<BigInt>);
    
    int _act = 1;
    int _total;
    int _index;
    int _max_rank;
    Vector<SoldierSprite*> * soldiers;
    std::set<int> equal_soldiers;
    std::set<int> lower_soldiers;


private:
    int rank;
    int speed;
    bool commander;
    int id;
    double shareKey;
    std::string privateKeyD;
    std::string publicKeyN;
    std::string publicKeyE;
    std::vector<std::string> publicKeysN;
    std::vector<std::string> publicKeysE;
    
    int _rank;
    BigInt _r;
    BigInt _p = BigInt(29);

};
#endif /* SoldierSprite_hpp */
