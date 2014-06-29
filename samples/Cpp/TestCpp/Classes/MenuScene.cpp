//
//  MenuScene.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#include "MenuScene.h"
#include "UserInfo.h"
#include "SaveData.h"
MenuScene::MenuScene(){}
MenuScene::~MenuScene(){}


Scene * MenuScene::scene(){
    Scene* scene = Scene::create();
    MenuScene *layer = MenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MenuScene::init(){
    if (!Layer::init()) {
        return false;
    }
    
    int maxMissionId = SaveData::getInstance()->getMaxMissionId();
    for (int i=0; i<4; i++) {
        ControlButton * playButton = ControlButton::create();
        playButton->setTag(i);
        playButton->setPosition(Point(100 + i * 80,100));
        playButton->setBackgroundSpriteForState(Scale9Sprite::create("Images/btn-play-normal.png"), Control::State::NORMAL);
        playButton->setBackgroundSpriteForState(Scale9Sprite::create("Images/btn-play-selected.png"), Control::State::HIGH_LIGHTED);
        playButton->setBackgroundSpriteForState(Scale9Sprite::create("Images/btn-play-selected.png"), Control::State::DISABLED);
        playButton->setAdjustBackgroundImage(false);
        playButton->addTargetWithActionForControlEvents(this, cccontrol_selector(MenuScene::goToMission), Control::EventType::TOUCH_UP_INSIDE);
        this->addChild(playButton);
        
        if (i> maxMissionId + 1) {
            playButton->setEnabled(false);
        }
    }
    
    return true;
}

void MenuScene::goToMission(Object* sender, Control::EventType event){
    Node *node = dynamic_cast<Node*>(sender);
    int tag = node->getTag();
    log("test %d", tag);
    
    UserInfo::getInstance()->setCurMissionId(tag);
    //CCDirector::getInstance()->replaceScene(BattleScene::scene());
}