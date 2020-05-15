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

using namespace cocos2d;

class SoldierSprite : public Sprite{
public:
    SoldierSprite();
    static SoldierSprite* create(const std::string& filename, const Rect& rect);
    void setSpeed(int _speed) {speed = _speed;}
    void setRank(int _rank) {rank = _rank;}
    void setPrivateKey(std::string _privateKey) {privateKey = _privateKey;}
    void setPublicKey(std::string _publicKey) {publicKey = _publicKey;}
    void setPublicKeys(std::vector<std::string> _publicKeys) {publicKeys = _publicKeys;}
    int getSpeed(){return speed;}
    int getRank(){return rank;}
    std:: string getRankS() {return std::to_string(rank);}
    std::string getPrivateKey() {return privateKey;}
    std::string getPublicKey() {return publicKey;}
    std::vector<std::string> getPublicKeys() {return publicKeys;}
    Label* info;
    
    std::string rsaEncryption(std::string plainText, std::string N, int id);
    std::string rsaDecryption(std::string ciperText, std::string N);
    
    void messageProcess(std::string msg,Vec2 Pos);


private:
    int rank;
    int speed;
    bool commander;
    std::string privateKey;
    std::string publicKey;
    std::vector<std::string> publicKeys;
};
#endif /* SoldierSprite_hpp */
