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
    int getSpeed(){return speed;}
    int getRand(){return rank;}
    
private:
    int rank;
    int speed;
    
};
#endif /* SoldierSprite_hpp */
