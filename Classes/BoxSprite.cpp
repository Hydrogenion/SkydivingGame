//
//  BoxSprite.cpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/21.
//

#include "BoxSprite.hpp"
BoxSprite::BoxSprite()
{
#if CC_SPRITE_DEBUG_DRAW
    _debugDrawNode = DrawNode::create();
    addChild(_debugDrawNode);
#endif //CC_SPRITE_DEBUG_DRAW
}


BoxSprite* BoxSprite::create(const std::string& filename, const Rect& rect)
{
    BoxSprite *sprite = new (std::nothrow) BoxSprite();
    if (sprite && sprite->initWithFile(filename, rect))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
    
}
