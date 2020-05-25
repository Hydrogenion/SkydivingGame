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

BigInt randomBigInteger_soldier(int len) {
    if (len == 0) return BigInt("0");
    if (len == 1) return BigInt("1");
    BigInt x("1");
    for (int i = 1; i < len; i++)
        x = x * BigInt("2");
    std::string s(len, '0');
    for (int i = 0; i < s.length(); i++)
        s[i] += rand() % 10;
    BigInt res = BigInt(s) % x + x;
    if (res % BigInt("2") == BigInt("0"))
        return res + BigInt("1");
    else
        return res;
}

SoldierSprite::SoldierSprite()
{
#if CC_SPRITE_DEBUG_DRAW
    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode);
#endif //CC_SPRITE_DEBUG_DRAW
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

BigInt SoldierSprite::rsaEncryptionBigInt(BigInt plainText, int id){
    std::string N, E;
    N = publicKeysN[id];
    E = publicKeysE[id];

    BigInt ciperText = power_soldier(plainText, BigInt(E, 16), BigInt(N, 16));
    return ciperText;
}

BigInt SoldierSprite::rsaDecryptionBigInt(BigInt ciperText){
    BigInt plainText = power_soldier(ciperText, BigInt(privateKeyD, 16), BigInt(publicKeyN, 16));
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

int SoldierSprite::compare_once() {
    // pick a soldier to compare
    int select;
    do {
        select = rand() % soldiers->size();
    } while (!(select != _index && equal_soldiers.find(select) == equal_soldiers.end() &&
                lower_soldiers.find(select) == lower_soldiers.end()));
    printf("Soldier %d selects Soldier %d: ", _index, select);
    
//    Vec2 index_pos = soldiers->at(_index)->getPosition();
//    Vec2 pos = soldiers->at(select)->getPosition();
//    auto moveTo = MoveTo::create(0.5,pos);
//    auto moveBack = MoveTo::create(0.5,index_pos);
//    this->runAction(moveTo);
//    this->runAction(moveBack);
    
    SoldierSprite& select_soldier = *(soldiers->at(select));

    if (!select_soldier.require_comparison()) {
        printf("Refuse comparison!\n");
    } else {
        int result = compare(*this, select_soldier, _index, select);
        if (result >= 0) {
            select_soldier._act = 0;
            if (result == 0) {
                printf("Equal!\n");
                equal_soldiers.insert(select);
                for (std::set<int>::iterator it = select_soldier.equal_soldiers.begin(); it != select_soldier.equal_soldiers.end(); ++it)
                    equal_soldiers.insert(*it);
            } else {
                printf("%d Win!\n", _index);
                lower_soldiers.insert(select);
                for (std::set<int>::iterator it = select_soldier.equal_soldiers.begin(); it != select_soldier.equal_soldiers.end(); ++it)
                    lower_soldiers.insert(*it);
            }
            for (std::set<int>::iterator it = select_soldier.lower_soldiers.begin(); it != select_soldier.lower_soldiers.end(); ++it)
                lower_soldiers.insert(*it);

            if (equal_soldiers.size() + lower_soldiers.size() + 1 == _total)
                return _index;
        }
        else {
            _act = 0;
            select_soldier.lower_soldiers.insert(_index);
            for (std::set<int>::iterator it = lower_soldiers.begin(); it != lower_soldiers.end(); ++it)
                select_soldier.lower_soldiers.insert(*it);
            for (std::set<int>::iterator it = equal_soldiers.begin(); it != equal_soldiers.end(); ++it)
                select_soldier.lower_soldiers.insert(*it);

            printf("%d Win!\n", select);

            if (select_soldier.equal_soldiers.size() + select_soldier.lower_soldiers.size() + 1 == _total)
                return select;
        }
    }
    return -1;
}

bool SoldierSprite::require_comparison() {
    return _act == 1;
}

int SoldierSprite::compare(SoldierSprite& A, SoldierSprite& B, int index_A, int index_B) {
    bool result1 = A.step3(B.step2(A.step1(index_B)));
    if (result1) { // A <= B
        bool result2 = B.step3(A.step2(B.step1(index_A)));
        if (result2) {  // B <= A -> A == B
            return 0;
        } else {    // B > A
            return -1;
        }
    } else {    // A > B
        return 1;
    }
}

BigInt SoldierSprite::step1(int to_index) {
    BigInt r = randomBigInteger_soldier(1024);
    _r = r;
//    return r - BigInt(_rank);
//    return BigInt(rsaEncryption(to_string_hex(r), to_index),16) - BigInt(_rank);
    return rsaEncryptionBigInt(r,to_index) - BigInt(_rank);   //todo: encrypt using to_index's public key
}

std::vector<BigInt> SoldierSprite::step2(BigInt x) {
    std::vector<BigInt> v;
    for(int i = 1; i <= _max_rank; ++i) {
        v.push_back(rsaDecryptionBigInt(x + BigInt(i)) % _p);     //todo: decrypt with private key
//        BigInt tmp = (x + BigInt(i))%_p;
//        v.push_back(BigInt(rsaDecryption(to_string_hex(tmp)),16));
//        v.push_back((x+BigInt(i))%_p);
    }
    for (int i = _rank + 1; i <= _max_rank; ++i) {
        v[i - 1] = v[i - 1] + BigInt(1);
    }
    return v;
}

bool SoldierSprite::step3(std::vector<BigInt> v) {
    return v[_rank - 1]  == _r % _p;
}
