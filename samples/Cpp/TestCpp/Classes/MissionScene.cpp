//
//  MissionScene.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-30.
//
//

#include "MissionScene.h"
const float VELOCITY = 1.f;

MissionScene::MissionScene(){};
MissionScene::~MissionScene(){};

Scene* MissionScene::scene(){
    Scene* scene = Scene::create();
    MissionScene *layer = MissionScene::create();
    scene->addChild(layer);
    return scene;
}

bool MissionScene::init(){
    if (!Layer::init()) {
        return false;
    }
    
    //加载导出的文件
    ArmatureDataManager::getInstance()->addArmatureFileInfo( "DemoPlayer/DemoPlayer0.png" , "DemoPlayer/DemoPlayer0.plist" , "DemoPlayer/DemoPlayer.ExportJson" );
    ArmatureDataManager::getInstance()->addArmatureFileInfo( "DemoPlayer/DemoPlayer1.png" , "DemoPlayer/DemoPlayer1.plist" , "DemoPlayer/DemoPlayer.ExportJson" );
    //创建对象
    Armature *unit0 = Armature::create( "DemoPlayer" );
    Armature * unit1 = Armature::create( "DemoPlayer" );
    Armature * unit2 = Armature::create( "DemoPlayer" );
    //调用动画
    unit0->getAnimation()->playWithIndex(1);
    unit1->getAnimation()->playWithIndex(1);
    unit2->getAnimation()->playWithIndex(1);
    unit0->setScale(.1f);
    unit1->setScale(.1f);
    unit2->setScale(.1f);

    scheduleUpdate();
    
    // Adds Touch Event Listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(MissionScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MissionScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MissionScene::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -10);
    _touchListener = listener;
    
    //queue
    ArmatureDataManager::getInstance()->addArmatureFileInfo( "Hero/Hero0.png" , "Hero/Hero0.plist" , "Hero/Hero.ExportJson" );
    
    queue = Armature::create( "Hero" );
    Armature *hero = queue;
    this->addChild(hero);
    hero->setPosition(200,200);
    hero->getAnimation()->playWithIndex(0);
    
    firstUnit = hero->getBone("Layer13");
    secondUnit = hero->getBone("Layer3");
    thirdUnit = hero->getBone("Layer4");
    hero->getBone("Layer13")->addDisplay(unit0, 0);
    hero->getBone("Layer3")->addDisplay(unit1, 0);
    hero->getBone("Layer4")->addDisplay(unit2, 0);
    
    return true;
}

bool MissionScene::onTouchBegan(Touch* touch, Event* event){
    finalPos = touch->getLocation();
    
    Point startPos = queue->getPosition();
    slope = (finalPos.y - startPos.y) / (finalPos.x - startPos.x);
    log("start x=%f y=%f end x=%f y=%f", startPos.x, startPos.y, finalPos.x, finalPos.y);
    log("slope=%f", slope);
    
    //调整队伍角度 注意rotation是相对父节点的
    float lastRotation = finalRotation;
    finalRotation = 3.14-atan2(finalPos.y - startPos.y, finalPos.x - startPos.x);
    log("fianlRotation=%f", finalRotation);
    //判断顺时针 还是逆时针
    if (finalRotation > lastRotation) {
        if (finalRotation - lastRotation > 1.57) {
            isClockWise = false;
        }else{
            isClockWise = true;
        }
    }else{
        if (finalRotation + 2 * 3.14 - lastRotation > 1.57) {
            isClockWise = false;
        }else{
            isClockWise = true;
        }
    }
    
    log("first rotation=%f, finalRotation=%f", firstUnit->getRotation(), finalRotation);
    
    vX = sqrt(VELOCITY * VELOCITY / (slope * slope + 1));
    
    vX = finalPos.x > startPos.x ? vX : -vX;
    
    vY = vX * slope;

    return true;
}
void MissionScene::onTouchMoved(Touch* touch, Event* event){
    
}
void MissionScene::onTouchEnded(Touch* touch, Event* event){
    
}

void MissionScene::update (float deltaTime) {
    
    Point nowPos = firstUnit->getPosition();
    bool keepGoing = false;
    if (vX > 0) {
        keepGoing = finalPos.x > nowPos.x;
    }else{
        keepGoing = finalPos.x < nowPos.x;
    }
    
    if (vY > 0) {
        keepGoing = finalPos.y > nowPos.y;
    }else{
        keepGoing = finalPos.y < nowPos.y;
    }
    
    if (keepGoing){
        firstUnit->setPosition(firstUnit->getPosition() + Point(vX, vY));
    }
//    
//    if (int(firstUnit->getRotation() + finalRotation) % 360) {
//        firstUnit->setRotation( isClockWise ? firstUnit->getRotation() - 1 : firstUnit->getRotation() + 1 );
//        secondUnit->setRotation( isClockWise ? secondUnit->getRotation() + 0.5 : secondUnit->getRotation() - 0.5 );
//    }else{
//        
//        if (int(secondUnit->getRotation()) % 360) {
//            secondUnit->setRotation( isClockWise ? secondUnit->getRotation() - 1 : secondUnit->getRotation() + 1 );
//        }
//    }
    
}