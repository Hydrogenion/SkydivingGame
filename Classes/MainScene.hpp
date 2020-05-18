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
#include "ui/CocosGUI.h"
#include "rsa.h"
using namespace cocos2d;
class MainScene : public Layer,cocos2d::ui::EditBoxDelegate{
public:
    static Scene* createscene();//创建场景的架构
    virtual bool init();   //myfirstscene的实现
    CREATE_FUNC(MainScene);   //生成myfirstscene的create
    void comeback(Ref * Psendeer);  //回调函数
    void update(float delta);
    bool isKeyPressed(EventKeyboard::KeyCode keyCode);
    void keyActionS();
    void keyPressedDuration(EventKeyboard::KeyCode code);
    
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox,const std::string &text);
    virtual void editBoxReturn(cocos2d::ui::EditBox *editBox);
    
    //在这里初始化你的函数
    void initRSA();
    void initMillionaire();
    void initVoting();
    void intiKeySharing();
    
    //RSA的部分
    std::vector<std::string> rsaGenerateKey(int keyLen);
    std::vector<std::vector<std::string>> gengerateKeysAGroup(int groupSize);
    std::vector<std::string> getAllPublicKey(std::vector<std::vector<std::string>> keysPV);
    
    // Millionaire部分
    void millionaireMain();

    
private:
    std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
    Vector<SoldierSprite*> soldierBlue;
    Vector<SoldierSprite*> soldierRed;
    Vector<Label*> infoBlue;
    int count = 0;
    bool editingMode = false;
    std::string messageToSend = "";
    std::string N;
};

#endif /* MainScene_hpp */
