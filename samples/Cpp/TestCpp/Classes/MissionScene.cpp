//
//  MissionScene.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-30.
//
//

#include "MissionScene.h"

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
    
    
    //先加载队列
    ArmatureDataManager::getInstance()->addArmatureFileInfo( "Hero/Hero0.png" , "Hero/Hero0.plist" , "Hero/Hero.ExportJson" );
    Armature *queue = Armature::create( "Hero" );
    queue->setPosition(200,100);
    this->addChild(queue);
    
    //加载导出的文件
    ArmatureDataManager::getInstance()->addArmatureFileInfo( "DemoPlayer/DemoPlayer0.png" , "DemoPlayer/DemoPlayer0.plist" , "DemoPlayer/DemoPlayer.ExportJson" );
    ArmatureDataManager::getInstance()->addArmatureFileInfo( "DemoPlayer/DemoPlayer1.png" , "DemoPlayer/DemoPlayer1.plist" , "DemoPlayer/DemoPlayer.ExportJson" );
    //创建对象
    Armature *unit0 = Armature::create( "DemoPlayer" );
    Armature *unit1 = Armature::create( "DemoPlayer" );
    Armature *unit2 = Armature::create( "DemoPlayer" );
    //调用动画
    unit0->getAnimation()->playWithIndex(1);
    unit1->getAnimation()->playWithIndex(1);
    unit2->getAnimation()->playWithIndex(1);
    unit0->setScale(.1f);
    unit1->setScale(.1f);
    unit2->setScale(.1f);
    //unit0->setPosition(100,100);
    //unit1->setPosition(200,200);
//    this->addChild(unit0);
//    this->addChild(unit1);
    queue->getBone("Layer13")->addDisplay(unit0, 0);
    queue->getBone("Layer3")->addDisplay(unit1, 0);
    queue->getBone("Layer4")->addDisplay(unit2, 0);
    return true;
}