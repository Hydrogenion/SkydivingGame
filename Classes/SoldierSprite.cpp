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
