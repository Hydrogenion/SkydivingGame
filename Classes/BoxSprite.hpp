//
//  BoxSprite.hpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/21.
//

#ifndef BoxSprite_hpp
#define BoxSprite_hpp

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class BoxSprite : public Sprite{
public:
    BoxSprite();
    static BoxSprite* create(const std::string& filename, const Rect& rect);

private:
    int rank;

};
#endif /* BoxSprite_hpp */
