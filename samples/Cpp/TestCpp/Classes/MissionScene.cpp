//
//  MissionScene.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-30.
//
//

#include "MissionScene.h"

const float VELOCITY = 1.f;

MissionScene::MissionScene(){
    m_units = NULL;
};
MissionScene::~MissionScene(){
    CC_SAFE_RELEASE_NULL(m_units);
};

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
    this->setUnits(__Array::create());
    //加载导出的文件
    ArmatureDataManager::getInstance()->addArmatureFileInfo( "DemoPlayer/DemoPlayer0.png" , "DemoPlayer/DemoPlayer0.plist" , "DemoPlayer/DemoPlayer.ExportJson" );
    ArmatureDataManager::getInstance()->addArmatureFileInfo( "DemoPlayer/DemoPlayer1.png" , "DemoPlayer/DemoPlayer1.plist" , "DemoPlayer/DemoPlayer.ExportJson" );
    //创建对象
    for (int i=0; i<QUEUE_COUNT; i++) {
        Armature * unit = Armature::create( "DemoPlayer" );
        unit->getAnimation()->playWithIndex(1);
        unit->setScale(.1f);
        this->getUnits()->addObject(unit);
        unit->setPosition(200 + 50 * i, 200);
        this->addChild(unit);
    }
    
    scheduleUpdate();
    
    // Adds Touch Event Listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(MissionScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MissionScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MissionScene::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -10);
    _touchListener = listener;
    
    return true;
}

bool MissionScene::onTouchBegan(Touch* touch, Event* event){
    Point endPos = touch->getLocation();
    
    for (int i=0; i<this->getUnits()->count(); i++) {
        Armature * unitI = dynamic_cast<Armature*>(this->getUnits()->getObjectAtIndex(i));
        for (int j=i; j>0; j--) {
            Armature * unit = dynamic_cast<Armature*>(this->getUnits()->getObjectAtIndex(j-1));
            Point posForPath = unit->getPosition();
            
            m_paths[i][i-j] = posForPath;
            
        }
        
        m_paths[i][i] = endPos;
        
        m_vector[i] = getVector(unitI->getPosition(), m_paths[i][0]);
        
        m_currentTarget[i] = 0;
    }
    
    for (int i=0; i<10; i++) {
        for (int j=0; j<10; j++) {
            log("x=%f, y=%f", m_paths[i][j].x,m_paths[i][j].y);
        }
    }
    return true;
}

Point MissionScene::getVector(Point startPos, Point endPos){
    float x = 0;
    float y = 0;
    if(endPos.x == startPos.x){
        x = 0;
        y = endPos.y > startPos.y ? VELOCITY : -VELOCITY;
    }else{
        float slope = (endPos.y - startPos.y) / (endPos.x - startPos.x);
        x = sqrt(VELOCITY * VELOCITY / (slope * slope + 1));
        
        x = endPos.x > startPos.x ? x : -x;
        y = x * slope;
    }
    return Point(x, y);
}

void MissionScene::onTouchMoved(Touch* touch, Event* event){
    
}
void MissionScene::onTouchEnded(Touch* touch, Event* event){
    
}

void MissionScene::update (float deltaTime) {
    for (int i=0; i<this->getUnits()->count(); i++) {
        Armature * unit = dynamic_cast<Armature*>(this->getUnits()->getObjectAtIndex(i));
        Point vector = m_vector[i];
        //翻面
        if (vector.x>0) {
            unit->setScaleX(-.1f);
        }else{
            unit->setScaleX(.1f);
        }
        unit->setPosition(vector + unit->getPosition());
        
        //判断是否换target
        int currentIndex = m_currentTarget[i];
        if (currentIndex == i) {//继续走
            
        }else{//看情况换target
            bool changeTarget = false;
            Point currentTarget = m_paths[i][currentIndex];
            Point currentPos = unit->getPosition();
            if (vector.x == 0) {
                if (vector.y >0) {
                    if (currentPos.y >= currentTarget.y) {
                        changeTarget = true;
                    }
                }else{
                    if (currentPos.y <= currentTarget.y) {
                        changeTarget = true;
                    }
                }
            }else{
                if (vector.x > 0) {
                    if (currentPos.x >= currentTarget.x) {
                        changeTarget = true;
                    }
                }else{
                    if (currentPos.x <= currentTarget.x) {
                        changeTarget = true;
                    }
                }
            }
            
            if (changeTarget) {
                m_currentTarget[i] = currentIndex + 1;
                m_vector[i] = getVector(unit->getPosition(), m_paths[i][currentIndex+1]);
            }
        }
    }
    
    
}