//
//  MissionScene.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-30.
//
//

#include "MissionScene.h"

const float VELOCITY = 1.f;
const float GAP = 50.f;
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
        unit->setPosition(200 + GAP * i, 200);
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
    
    m_paths[0] = dynamic_cast<Armature*>(this->getUnits()->getObjectAtIndex(0))->getPosition();
    return true;
}

bool MissionScene::onTouchBegan(Touch* touch, Event* event){
    Point endPos = touch->getLocation();
    
    
    m_paths[m_paths.size()-1] = dynamic_cast<Armature*>(this->getUnits()->getObjectAtIndex(0))->getPosition();
    m_paths[m_paths.size()] = endPos;
    for (int i=0; i<m_paths.size(); i++) {
        log("path i=%d x=%f y=%f", i, m_paths[i].x, m_paths[i].y);
    }
    
    for (int i=0; i<getUnits()->count(); i++) {
        int currentTaget = m_currentTarget[i];
        Armature * unit = dynamic_cast<Armature*>(getUnits()->getObjectAtIndex(i));
        m_vector[i] = getVector(unit->getPosition(), m_paths[currentTaget]);
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
    x = int(x * 100) / 100.f;
    y = int(y * 100) / 100.f;
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
        }else if(vector.x < 0){
            unit->setScaleX(.1f);
        }
        unit->setPosition(vector + unit->getPosition());
        
        int currentIndex = m_currentTarget[i];
        if (currentIndex == m_paths.size() - 1) {
            continue;
        }
        
        Point endPos = m_paths[currentIndex];
        Point nowPos = unit->getPosition();
        //m_vector[i] = getVector(nowPos, endPos);
 log("m_vector currentIndex=%d i=%d start x=%f y=%f end x=%f y=%f",currentIndex, i, nowPos.x, nowPos.y, endPos.x, endPos.y);
        vector = m_vector[i];
        //log("m_vector i=%d x=%f y=%f", i, vector.x, vector.y);
        bool changeTarget = false;

        if (vector.x == 0) {
            if (vector.y >0) {
                if (nowPos.y >= endPos.y) {
                    changeTarget = true;
                }
            }else{
                if (nowPos.y <= endPos.y) {
                    changeTarget = true;
                }
            }
        }else{
            if (vector.x > 0) {
                if (nowPos.x >= endPos.x) {
                    changeTarget = true;
                }
            }else{
                if (nowPos.x <= endPos.x) {
                    changeTarget = true;
                }
            }
        }
        
        if (changeTarget) {
            
            log("changetarget i=%d currentId=%d", i, currentIndex);
            m_currentTarget[i] = currentIndex + 1;
            m_vector[i] = getVector(unit->getPosition(), m_paths[m_currentTarget[i]]);
            
            //调整位置
            if (i>0) {
                Armature *lastUnit = dynamic_cast<Armature*>( getUnits()->getObjectAtIndex(i-1) );
                Point lastPos = lastUnit->getPosition();
                Point slope = getVector(nowPos, lastPos);
                float tmpSlope = fabs(slope.y / slope.x);
                if (nowPos.getDistance(lastPos) != GAP) {
                    float x = sqrtf(GAP * GAP / (tmpSlope * tmpSlope + 1));
                    float finalX = slope.x > 0 ? lastPos.x - x : lastPos.x + x;
                    float finalY = slope.y > 0 ? lastPos.y - tmpSlope * x : lastPos.y + tmpSlope * x;
                    unit->setPosition(finalX, finalY);
                }
            }
            
        }
    }
}