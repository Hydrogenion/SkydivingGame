//
//  SoldierSprite.cpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/13.
//

#include "SoldierSprite.hpp"

//SoldierSprite::SoldierSprite(){
//    rank = rand()%10;
//    speed = 5;
//}
bool isodd_soldier(BigInt b)
{
    return (b % 2) == 1;
}

BigInt power_soldier(BigInt a, BigInt b, BigInt m)
{
    if (b == BigInt("0")) return BigInt("1");
    if (b == BigInt("1")) return a % m;
    BigInt hb = b / 2;
    BigInt phb = power_soldier(a, hb, m);
    BigInt res = (phb * phb) % m;
    if (isodd_soldier(b))
        res = (a * res) % m;
    return res;
}

SoldierSprite::SoldierSprite()
{
#if CC_SPRITE_DEBUG_DRAW
    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode);
#endif //CC_SPRITE_DEBUG_DRAW
    rank = rand()%10;
    speed = 5;
}

//Sprite::~Sprite()
//{
//    CC_SAFE_FREE(_trianglesVertex);
//    CC_SAFE_FREE(_trianglesIndex);
//    CC_SAFE_RELEASE(_spriteFrame);
//    CC_SAFE_RELEASE(_texture);
//}


SoldierSprite* SoldierSprite::create(const std::string& filename, const Rect& rect)
{
    SoldierSprite *sprite = new (std::nothrow) SoldierSprite();
    if (sprite && sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
    
}

std::string SoldierSprite::rsaEncryption(std::string plainText, int id){
    std::string N, E;
    N = publicKeysN[id];
    E = publicKeysE[id];

    std::string ciperText = to_string_hex(power_soldier(BigInt(plainText, 16), BigInt(E, 16), BigInt(N, 16)));
    return ciperText;
}

std::string SoldierSprite::rsaDecryption(std::string ciperText){
    std::string plainText = to_string_hex(power_soldier(BigInt(ciperText, 16), BigInt(privateKeyD, 16), BigInt(publicKeyN, 16)));
    return plainText;
}


std::string SoldierSprite::sendIdentification() {
    std::string ciperText = to_string_hex(power_soldier(BigInt(id), BigInt(privateKeyD, 16), BigInt(publicKeyN, 16)));
    return ciperText;
}

bool SoldierSprite::recieveIdentification(std::string ciperText, int id) {
    std::string N, E;
    N = publicKeysN[id];
    E = publicKeysE[id];
    BigInt r = power_soldier(BigInt(ciperText, 16), BigInt(E, 16), BigInt(N, 16));
    BigInt gt = BigInt(id + 'a');

    if (r == gt)
        return true;
    else
        return false;
}

void SoldierSprite::messageProcess(std::string msg,Vec2 Pos){
//    auto moveTo = MoveTo::create(2, Vec2(50, 0));
    if (msg=="Follow"){
    auto moveTo = MoveTo::create(2,Pos);
    this->runAction(moveTo);
    }
}
