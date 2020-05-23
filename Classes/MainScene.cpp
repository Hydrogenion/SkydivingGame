//
//  MainScene.cpp
//  HelloWorld
//
//  Created by 张轩诚 on 2020/5/13.
//

#include "MainScene.hpp"
#include "utils.h"
#include "pthread.h"
#include <ctime>
#include "math.h"
#define OFFSET_Y 200

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
    auto background = Sprite::create("Background.jpg");
    // position the sprite on the center of the screen
    background->setPosition(Vec2(VisibleSize.width/2 + origin.x, VisibleSize.height/2 + origin.y));
    this->addChild(background, 0);
    
    infoLabel = Label::createWithSystemFont("", "fonts/arial.ttf", 13);
    infoLabel->setPosition(Vec2(545,90));
    infoLabel->setColor(Color3B(0,0,0));
    infoLabel->setAlignment(TextHAlignment::LEFT);
    infoLabel->setWidth(1000);
    infoLabel->setHeight(130);
    this->addChild(infoLabel,1);
    
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
    
        for (auto soldier : soldierAll){
            if ((cursorX>soldier->getPosition().x-16) && (cursorX<soldier->getPosition().x+16) && (cursorY>soldier->getPosition().y-16) && (cursorY<soldier->getPosition().y+16))
            {
                std::string ciperText = messageToSend;
                std::string plainText = ciperText;
                if (messageToSend=="auth")
                {
                    std::string txt = soldier->sendIdentification();
                    bool flag = soldierBlue.front()->recieveIdentification(txt, soldier->getID()-'a');
                    if (flag==true){
                        soldier->setTexture("soldier1.png");
                        soldier->setTextureRect(Rect(0,0,32,32));
                    }
                    else {
                        soldier->setTexture("soldier2.png");
                        soldier->setTextureRect(Rect(0,0,32,32));
                    }
                }
                
                soldier->messageProcess(plainText,soldierBlue.front()->getPosition());
                
                labelCiper->setString(ciperText);
                labelCiper->setPosition(Vec2(cursorX,cursorY));
                labelPlain->setString(plainText);
                labelPlain->setPosition(Vec2(cursorX,cursorY+20));
            }
            id++;
        }
        
        for (auto box: Boxs){
            std::vector<double> idInfo;
            std::vector<double> shareKeyInfo;
            
            if ((cursorX>box->getPosition().x-15) && (cursorX<box->getPosition().x+15) && (cursorY>box->getPosition().y-15) && (cursorY<box->getPosition().y+15))
            {
                double box_x = box->getPosition().x;
                double box_y = box->getPosition().y;
                for (auto soldier : soldierAll){
                    float soldier_x = soldier->getPosition().x;
                    float soldier_y = soldier->getPosition().y;
                    float dis = sqrt((box_x-soldier_x)*(box_x-soldier_x)+(box_y-soldier_y)*(box_y-soldier_y));
                    if (dis<20){
                        idInfo.push_back(soldier->getID()-'a');
                        shareKeyInfo.push_back(soldier->getShareKey());
                    }
                }
                if (box->isOpen(idInfo, shareKeyInfo))
                {
                    box->setTexture("boxopen.png");
                }
                std::cout<<Boxs.getIndex(box)<<":"<<box->isOpen(idInfo, shareKeyInfo)<<std::endl;
            }
        }
    };
    
    mouseListener->onMouseUp = [=] (Event * event){
//        log("mouse up");
    };
    
    // initBox
    initBox();
    
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
void MainScene::initSoldierSprite(){
    //生成蓝军的密钥
    info_print("Start gengerate keys of blue");
    clock_t startTime, endTime;
    startTime = clock();
    auto keysOfBlue = gengerateKeysAGroup(10);
    auto publicKeyOfBlueE = getAllPublicKeyE(keysOfBlue);
    auto publicKeyOfBlueN = getAllPublicKeyN(keysOfBlue);
    endTime = clock();
    info_print("Time cost:"+std::to_string((double)(endTime - startTime) / CLOCKS_PER_SEC));
    //生成红军的密钥
    info_print("Start gengerate keys of red");
    startTime = clock();
    auto keysOfRed = gengerateKeysAGroup(10);
    auto publicKeyOfRedE = getAllPublicKeyE(keysOfBlue);
    auto publicKeyOfRedN = getAllPublicKeyN(keysOfBlue);
    endTime = clock();
    info_print("Time cost:"+std::to_string((double)(endTime - startTime) / CLOCKS_PER_SEC));
    
    info_print("Gengerate blue troops");
    //生成蓝军
    for (int i=0;i<10;i++){
        auto spriteSoldier = SoldierSprite::create("unknown.png",Rect(0,0,32,32));
        spriteSoldier->setPosition(Vec2(int(rand()%1080),int(rand()%720)+OFFSET_Y));
        spriteSoldier->info = Label::createWithSystemFont("", "fonts/arial.ttf", 13);
        spriteSoldier->info->setColor(Color3B(0,0,0));
        spriteSoldier->info->setString("");
        //        spriteSoldier->info->setString(std::string("Rank: ")+spriteSoldier->getRankS());
        spriteSoldier->info->setPosition(Vec2(spriteSoldier->getPosition().x,spriteSoldier->getPosition().y-25));
        //生成每个人公钥 与 私钥
        spriteSoldier->setPublicKeyN(keysOfBlue[i][0]);
        spriteSoldier->setPublicKeyE(keysOfBlue[i][1]);
        spriteSoldier->setPrivateKeyD(keysOfBlue[i][2]);
        spriteSoldier->setPublicKeysE(publicKeyOfBlueE);
        spriteSoldier->setPublicKeysN(publicKeyOfBlueN);
        spriteSoldier->setId('a' + i);
        spriteSoldier->setShareKey(Boxs.front()->generateShareKey(spriteSoldier->getID()-'a'));
        
        //将当前精灵放入队伍
        soldierBlue.pushBack(spriteSoldier);
        soldierAll.pushBack(spriteSoldier);
        //        infoBlue.pushBack(info);
        this->addChild(spriteSoldier,1);
        this->addChild(spriteSoldier->info,2);
    }
    info_print("Gengerate red troops");
    //生成红军
    for (int i=0;i<10;i++){
        auto spriteSoldier = SoldierSprite::create("unknown.png",Rect(0,0,32,32));
        spriteSoldier->setPosition(Vec2(int(rand()%1080),int(rand()%720)+OFFSET_Y));
        spriteSoldier->info = Label::createWithSystemFont("", "fonts/arial.ttf", 13);
        spriteSoldier->info->setColor(Color3B(0,0,0));
        spriteSoldier->info->setString("");
        spriteSoldier->info->setPosition(Vec2(spriteSoldier->getPosition().x,spriteSoldier->getPosition().y-25));
        //生成每个人公钥 与 私钥
        spriteSoldier->setPublicKeyN(keysOfRed[i][0]);
        spriteSoldier->setPublicKeyE(keysOfRed[i][1]);
        spriteSoldier->setPrivateKeyD(keysOfRed[i][2]);
        spriteSoldier->setPublicKeysE(publicKeyOfRedE);
        spriteSoldier->setPublicKeysN(publicKeyOfRedN);
        spriteSoldier->setId('a' + i);
        spriteSoldier->setShareKey(Boxs.back()->generateShareKey(spriteSoldier->getID()-'a'));
        //将当前精灵放入队伍
        soldierRed.pushBack(spriteSoldier);
        soldierAll.pushBack(spriteSoldier);
        this->addChild(spriteSoldier,1);
    }
    //生成主角
    soldierBlue.front()->setTexture("soldier1.png");
    soldierBlue.front()->setTextureRect(Rect(0,0,32,32));
    
//    std::vector<double> idInfo;
//    std::vector<double> shareKeyInfo;
//    for (int kk=0;kk<5;kk++){
//        idInfo.push_back(soldierBlue.at(kk)->getID()-'a');
//        shareKeyInfo.push_back(soldierBlue.at(kk)->getShareKey());
//    }
//    std::cout<<Boxs.at(0)->isOpen(idInfo, shareKeyInfo)<<std::endl;
//    std::cout<<Boxs.at(1)->isOpen(idInfo, shareKeyInfo)<<std::endl;
    info_print("You are the blue guy");
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
    infoLabel->setString(infoText);
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
            if ((pos.y>=720-16+OFFSET_Y) || editingMode==true)
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
    
    if (messageToSend=="init" && soldierAll.size()==0){
        initRSA(1024);
        pthread_t tid;
        pthread_create(&tid, NULL, &threadFunction, this);
    }
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

void MainScene::initRSA(int keyLen){
    P = generatePrime(keyLen / 2);
    Q = generatePrime(keyLen / 2);
    while (P == Q) Q = generatePrime(keyLen / 2);
    N = P * Q;
    fai = (P - BigInt("1")) * (Q - BigInt("1"));
    return;
}

std::vector<std::string> MainScene::rsaGenerateKey(int keyLen){
    std::vector<std::string> vec;
    BigInt D, E;
    E = randomBigInteger(rand() % (keyLen));
    while (!(gcd(E, fai) == BigInt("1")))
    E = E + BigInt("2");
    D = modReverse(E, fai);
    
    vec.push_back(to_string_hex(N));
    vec.push_back(to_string_hex(E));
    vec.push_back(to_string_hex(D));
    
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

std::vector<std::string> MainScene::getAllPublicKeyE(std::vector<std::vector<std::string>> keysPV){
    std::vector<std::string> keyOfPublicKey;
    for (auto key : keysPV){
        keyOfPublicKey.push_back(key[1]);
    }
    return keyOfPublicKey;
}

std::vector<std::string> MainScene::getAllPublicKeyN(std::vector<std::vector<std::string>> keysPV) {
    std::vector<std::string> keyOfPublicKey;
    for (auto key : keysPV) {
        keyOfPublicKey.push_back(key[0]);
    }
    return keyOfPublicKey;
}
void MainScene::info_print(std::string str){
    infoText+=str;
    infoText+='\n';
}
void* MainScene::threadFunction(void *arg){
    MainScene *ptr = (MainScene*)arg;
    ptr->initSoldierSprite();
}

void MainScene::initBox(){
    info_print("Gengerate boxs");
    //生成宝箱
    for(int i=0;i<2;i++){
        auto spriteBox = BoxSprite::create("boxclose.png",Rect(0,0,31,31));
        spriteBox->setPosition(Vec2(int(rand()%1080),int(rand()%720)+OFFSET_Y));
        this->addChild(spriteBox,1);
        Boxs.pushBack(spriteBox);
        
        std::vector<double >coefficientInfo;
        for(int j = 0; j < 5; ++j) {
            coefficientInfo.push_back(double(rand() % 100 + 1));
        }
        spriteBox->setCoefficientInfo(coefficientInfo);
    }
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
