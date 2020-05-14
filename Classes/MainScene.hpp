//
//  MainScene.hpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/13.
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include <iostream>
#include <time.h>
#include "SoldierSprite.hpp"
using namespace cocos2d;
class MainScene : public Layer{
public:
    static Scene* createscene();//创建场景的架构
    virtual bool init();   //myfirstscene的实现
    CREATE_FUNC(MainScene);   //生成myfirstscene的create
    void comeback(Ref * Psendeer);  //回调函数
    void update(float delta);
    bool isKeyPressed(EventKeyboard::KeyCode keyCode);
    void keyActionS();
    void keyPressedDuration(EventKeyboard::KeyCode code);
    
private:
    std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
    Vector<SoldierSprite*> soldierBlue;
    Vector<SoldierSprite*> soldierRed;
    int count = 0;
};

#endif /* MainScene_hpp */
