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
    void setId(int _id) { id = _id; }
    void setShareKey(double _shareKey) {shareKey = _shareKey;}
    
    int getSpeed(){return speed;}
    int getRank(){return rank;}
    std:: string getRankS() {return std::to_string(rank);}
    std::string getPrivateKeyD() {return privateKeyD; }
    std::string getPublicKeyN() {return publicKeyN; }
    std::string getPublicKeyE() { return publicKeyE; }
    double getShareKey() {return shareKey;}
    int getID(){return id;}
    Label* info;
    
    std::string rsaEncryption(std::string plainText, int id);
    std::string rsaDecryption(std::string ciperText);
    
    std::string sendIdentification();
    bool recieveIdentification(std::string ciperText, int id);
    
    void messageProcess(std::string msg,Vec2 Pos);


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
};
#endif /* SoldierSprite_hpp */
