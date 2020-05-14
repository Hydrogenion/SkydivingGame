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

 
    auto label = Label::createWithSystemFont("超级黑哥", "fonts/arial.ttf", 30);
    //创建文字按钮
    auto menuitem = MenuItemLabel::create(label,CC_CALLBACK_1(MainScene::comeback, this));
    //将创建好的按钮装入菜单条目 ,并回调函数
    auto menu = Menu::create(menuitem, NULL);
    label->setPosition(Vec2(origin.x,VisibleSize.height/4+origin.y));
    //将条目装入系统的菜单里
    this->addChild(menu);
        //将菜单在我的场景中显示
    
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("Background.jpg");
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(VisibleSize.width/2 + origin.x, VisibleSize.height/2 + origin.y));
    
    for (int i=0;i<10;i++){
        auto spriteSoldier = SoldierSprite::create("soldier1.png",Rect(0,0,32,32));
        spriteSoldier->setPosition(Vec2(int(rand()%1080),int(rand()%720)));
        soldierBlue.pushBack(spriteSoldier);
        this->addChild(spriteSoldier,1);
    }
    for (int i=0;i<10;i++){
        auto spriteSoldier = SoldierSprite::create("soldier2.png",Rect(0,0,32,32));
        spriteSoldier->setPosition(Vec2(int(rand()%1080),int(rand()%720)));
        soldierRed.pushBack(spriteSoldier);
        this->addChild(spriteSoldier,1);
    }
    
    // Keboard Event
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
        keys[keyCode] = true;
    };
    listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
        keys[keyCode] = false;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    this->scheduleUpdate();

    
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
            if (pos.y>=720-16)
                break;
            soldierBlue.front()->setPosition(Vec2(pos.x,pos.y+soldierBlue.front()->getSpeed()));
            break;
        case EventKeyboard::KeyCode::KEY_S:
            if (pos.y<=16)
                break;
            soldierBlue.front()->setPosition(Vec2(pos.x,pos.y-+soldierBlue.front()->getSpeed()));
            break;
        case EventKeyboard::KeyCode::KEY_A:
            if (pos.x<=16)
                break;
            soldierBlue.front()->setPosition(Vec2(pos.x-soldierBlue.front()->getSpeed(),pos.y));
            break;
        case EventKeyboard::KeyCode::KEY_D:
            if (pos.x>=1080-16)
                break;
            soldierBlue.front()->setPosition(Vec2(pos.x+soldierBlue.front()->getSpeed(),pos.y));
            break;
    }
    // 0.3s代表着动作从开始到结束所用的时间，从而显得不会那么机械。
//    auto moveTo = MoveTo::create(0.3, Vec2(hero->getPositionX() + offsetX, hero->getPositionY() + offsetY));
//    hero->runAction(moveTo);
}

