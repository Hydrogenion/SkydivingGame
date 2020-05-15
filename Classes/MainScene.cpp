//
//  MainScene.cpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/13.
//

#include "MainScene.hpp"
Scene* MainScene::createscene()   //架构我的场景
{
    auto scene = Scene::create();            //先申请场景
    auto myscene = MainScene::create();   //构造我的层
    scene->addChild(myscene); //将我的层在场景中显示
    return scene;   //返回场景  -方便切换场景
}
 
 
bool MainScene::init()  //实现myfistscene的层
{
    srand((unsigned)time(NULL));
    if (!Layer::init())
        //先判断是否实现了父类的init（）函数，如果未成功实现Layer：：init（）会返回false，取反
    {
        return false;
    }
 
    Size VisibleSize = Director::getInstance()->getVisibleSize();
        //获得屏幕大小
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

 
//    auto label = Label::createWithSystemFont("超级黑哥", "fonts/arial.ttf", 30);
    //创建文字按钮
//    auto menuitem = MenuItemLabel::create(label,CC_CALLBACK_1(MainScene::comeback, this));
    //将创建好的按钮装入菜单条目 ,并回调函数
//    auto menu = Menu::create(menuitem, NULL);
//    label->setPosition(Vec2(origin.x,VisibleSize.height/4+origin.y));
    //将条目装入系统的菜单里
//    this->addChild(menu);
        //将菜单在我的场景中显示
    
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("Background.jpg");
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(VisibleSize.width/2 + origin.x, VisibleSize.height/2 + origin.y));
    this->addChild(sprite, 0);

    initRSA();
    
    //生成蓝军的密钥
    auto keysOfBlue = gengerateKeysAGroup(10);
    auto publicKeyOfBlue = getAllPublicKey(keysOfBlue);
    auto keysOfRed = gengerateKeysAGroup(10);
    auto publicKeyOfRed = getAllPublicKey(keysOfRed);
    
    //生成蓝军
    for (int i=0;i<10;i++){
        auto spriteSoldier = SoldierSprite::create("soldier1.png",Rect(0,0,32,32));
        spriteSoldier->setPosition(Vec2(int(rand()%1080),int(rand()%720)));
        spriteSoldier->info = Label::createWithSystemFont("", "fonts/arial.ttf", 13);
        spriteSoldier->info->setColor(Color3B(0,0,0));
        spriteSoldier->info->setString(std::string("Rank: ")+spriteSoldier->getRankS());
    spriteSoldier->info->setPosition(Vec2(spriteSoldier->getPosition().x,spriteSoldier->getPosition().y-25));
        //生成每个人公钥 与 私钥
        auto keyVec = rsaGenerateKey(1024);
        spriteSoldier->setPublicKey(keysOfBlue[i][0]);
        spriteSoldier->setPrivateKey(keysOfBlue[i][1]);
        spriteSoldier->setPublicKeys(publicKeyOfBlue);
        
        //将当前精灵放入队伍
        soldierBlue.pushBack(spriteSoldier);
//        infoBlue.pushBack(info);
        this->addChild(spriteSoldier,1);
        this->addChild(spriteSoldier->info,2);
    }
    //生成红军
    for (int i=0;i<10;i++){
        auto spriteSoldier = SoldierSprite::create("soldier2.png",Rect(0,0,32,32));
        spriteSoldier->setPosition(Vec2(int(rand()%1080),int(rand()%720)));
        //生成每个人公钥 与 私钥
        auto keyVec = rsaGenerateKey(1024);
        spriteSoldier->setPublicKey(keyVec[0]);
        spriteSoldier->setPrivateKey(keyVec[1]);
        //将当前精灵放入队伍
        soldierRed.pushBack(spriteSoldier);
        this->addChild(spriteSoldier,1);
    }
    
    //生成宝箱
    auto spriteBox = Sprite::create("box.png",Rect(0,0,13,12));
    spriteBox->setPosition(Vec2(int(rand()%1080),int(rand()%720)));
    this->addChild(spriteBox,1);
    
    // 键盘事件
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
        keys[keyCode] = true;
    };
    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
        keys[keyCode] = false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // 每个人头上的明文 密文
    auto labelCiper = Label::createWithSystemFont("", "fonts/arial.ttf", 15);
    auto labelPlain = Label::createWithSystemFont("", "fonts/arial.ttf", 15);
    this->addChild(labelCiper,3);
    this->addChild(labelPlain,3);
    // Mouse Event
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](Event * event)
    {
        EventMouse * e = (EventMouse *)event;
        float cursorX = e->getCursorX();
        float cursorY = e->getCursorY();
        int id = 0;
        for (auto soldier : soldierBlue){
            if ((cursorX>soldier->getPosition().x-16) && (cursorX<soldier->getPosition().x+16) && (cursorY>soldier->getPosition().y-16) && (cursorY<soldier->getPosition().y+16))
            {
                // encryption
                std::string ciperText = soldierBlue.front()->rsaEncryption(messageToSend,N,id);
                
                // decryption
                std::string plainText = soldier->rsaDecryption(ciperText, N);
                
                // messageProcess
                soldier->messageProcess(plainText,soldierBlue.front()->getPosition());
                
                labelCiper->setString(ciperText);
                labelCiper->setPosition(Vec2(cursorX,cursorY));
                labelPlain->setString(plainText);
                labelPlain->setPosition(Vec2(cursorX,cursorY+20));
            }
            id++;
        }
    };
    
    mouseListener->onMouseUp = [=] (Event * event){
        log("mouse up");
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    
    //TextFeild
    auto editBox = cocos2d::ui::EditBox::EditBox::create(Size(250,50), "CloseNormal.png");
    editBox->setDelegate(this);
    editBox->setPosition(Vec2(1080/2,20));
    
    // add the sprite as a child to this layer
    this->scheduleUpdate();
    this->addChild(editBox,2);



    
    return true;
}
 
void MainScene::comeback(Ref * Psendeer)//回调函数
{
    Director::getInstance()->replaceScene(MainScene::createscene());//切换至mysecondscene场景
        //导演场景的切换：replaceScene（）函数
}

void MainScene::update(float delta){
    // Register an update function that checks to see if the CTRL key is pressed
    // and if it is displays how long, otherwise tell the user to press it
    if(isKeyPressed(EventKeyboard::KeyCode::KEY_W)) {
        keyPressedDuration(EventKeyboard::KeyCode::KEY_W);
    }
    if(isKeyPressed(EventKeyboard::KeyCode::KEY_S)) {
        keyPressedDuration(EventKeyboard::KeyCode::KEY_S);
    }
    if(isKeyPressed(EventKeyboard::KeyCode::KEY_A)) {
        keyPressedDuration(EventKeyboard::KeyCode::KEY_A);
    }
    if(isKeyPressed(EventKeyboard::KeyCode::KEY_D)) {
        keyPressedDuration(EventKeyboard::KeyCode::KEY_D);
    }
    if(isKeyPressed(EventKeyboard::KeyCode::KEY_ENTER)) {
        editingMode=false;
    }
    
    for (auto soldier : soldierBlue){
        soldier->info->setPosition(Vec2(soldier->getPosition().x,soldier->getPosition().y-25));
    }
    Node::update(delta);
}

bool MainScene::isKeyPressed(EventKeyboard::KeyCode keyCode) {
    if(keys[keyCode]) {
        return true;
    } else {
        return false;
    }
}

void MainScene::keyActionS(){
    Vec2 pos = soldierBlue.front()->getPosition();
    soldierBlue.front()->setPosition(Vec2(pos.x,pos.y-10));
}

void MainScene::keyPressedDuration(EventKeyboard::KeyCode code) {
    Vec2 pos = soldierBlue.front()->getPosition();
    switch (code) {
        case EventKeyboard::KeyCode::KEY_W:
            if ((pos.y>=720-16) || editingMode==true)
                break;
            soldierBlue.front()->setPosition(Vec2(pos.x,pos.y+soldierBlue.front()->getSpeed()));
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if ((pos.y<=16 || editingMode==true))
                break;
            soldierBlue.front()->setPosition(Vec2(pos.x,pos.y-+soldierBlue.front()->getSpeed()));
            break;
        case EventKeyboard::KeyCode::KEY_A:
            if ((pos.x<=16 || editingMode==true))
                break;
            soldierBlue.front()->setPosition(Vec2(pos.x-soldierBlue.front()->getSpeed(),pos.y));
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if ((pos.x>=1080-16 || editingMode==true))
                break;
            soldierBlue.front()->setPosition(Vec2(pos.x+soldierBlue.front()->getSpeed(),pos.y));
            break;
    }
    // 0.3s代表着动作从开始到结束所用的时间，从而显得不会那么机械。
//    auto moveTo = MoveTo::create(0.3, Vec2(hero->getPositionX() + offsetX, hero->getPositionY() + offsetY));
//    hero->runAction(moveTo);
}


void MainScene::editBoxEditingDidBegin(cocos2d::ui::EditBox *editBox)
{
    log("start edit");
}
void MainScene::editBoxEditingDidEnd(cocos2d::ui::EditBox *editBox)
{
    log("end edit");
}
void MainScene::editBoxReturn(cocos2d::ui::EditBox *editBox)
{
    log("editbox return");
    messageToSend = editBox->getText();
    editBox->setText("");
}
void MainScene::editBoxTextChanged(cocos2d::ui::EditBox *editBox,const std::string &text)
{
    auto editbox = (cocos2d::ui::EditBox*)editBox;
    switch(editBox->getTag())
    {
    case 101:
        log("EditBox_name changed");
//        m_name=text;
        break;
    case 102:
        log("EditBox_password changed");
//        m_password=text;
        break;
    default:
        break;
    }
}

void MainScene::initRSA(){
    //generate N
    N = "";
    return;
}

std::vector<std::string> MainScene::rsaGenerateKey(int keyLen){
    std::vector<std::string> vec;
    std::string publicKey ="p";
    std::string privateKey ="v";
    vec.push_back(publicKey);
    vec.push_back(privateKey);
    return vec;
}

std::vector<std::vector<std::string>> MainScene::gengerateKeysAGroup(int groupSize){
    std::vector<std::vector<std::string>> keys;
    for (int i=0;i<groupSize;i++){
        auto keyVec = rsaGenerateKey(1024);
        keys.push_back(keyVec);
    }
    return keys;
}

std::vector<std::string> MainScene::getAllPublicKey(std::vector<std::vector<std::string>> keysPV){
    std::vector<std::string> keyOfPublicKey;
    for (auto key : keysPV){
        keyOfPublicKey.push_back(key[0]);
    }
    return keyOfPublicKey;
}


//备用代码
//    textField->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
//
//    textField->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type)
//    {
//        editingMode = true;
//    });
    
//    //Button
//    auto button = cocos2d::ui::Button::create("CloseNormal.png", "CloseSelected.png", "CloseSelected.png");
//
//    button->setTitleText("Button Text");
//
//    button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
//            switch (type)
//            {
//                    case ui::Widget::TouchEventType::BEGAN:
//                            break;
//                    case ui::Widget::TouchEventType::ENDED:
//                            std::cout << "Button 1 clicked" << std::endl;
////                    editBox->getText();
////                    std::cout << editBox->getText() << std::endl;
////                    textField->setString(std::string(""));
//                            break;
//                    default:
//                            break;
//            }
//    });
//    button->setPosition(Vec2(1080/4*3,20));
