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
    /*
     test
     */
    //test sprite 碰撞
    enemy = Sprite::create("Images/b1.png");
    this->addChild(enemy);
    enemy->setPosition(200, 0);
    enemy->runAction(RepeatForever::create(Sequence::create(MoveTo::create(2.f, Point(200, 200)),MoveTo::create(2.f, Point(0, 200)), MoveTo::create(2.f, Point(0, 0)), MoveTo::create(2.f, Point(200, 0)), NULL)));
    
    //创建对象
    for (int i=0; i<QUEUE_COUNT; i++) {
        Armature * unit = Armature::create( "DemoPlayer" );
        unit->getAnimation()->playWithIndex(1);
        unit->setScale(.1f);
        this->getUnits()->addObject(unit);
        unit->setPosition(300 + 5 * i, 200);
        this->addChild(unit);
        
        /*
        float minx = 0, miny = 0, maxx = 0, maxy = 0;
        ColliderDetector *detector = unit->getBone("Layer14")->getColliderDetector();
        if(detector){
            const cocos2d::Vector<ColliderBody*>& bodyList = detector->getColliderBodyList();
            Object *object = NULL;
            for (auto& object : bodyList)
            {
                ColliderBody *body = static_cast<ColliderBody*>(object);
                const std::vector<Point> &vertexList = body->getCalculatedVertexList();
                
                unsigned long length = vertexList.size();
                Point *points = new Point[length];
                for (int j = 0; j<length; j++)
                {
                    Point p = vertexList.at(j);
                    if (j == 0)
                    {
                        minx = maxx = p.x;
                        miny = maxy = p.y;
                    }
                    else
                    {
                        minx = p.x < minx ? p.x : minx;
                        miny = p.y < miny ? p.y : miny;
                        maxx = p.x > maxx ? p.x : maxx;
                        maxy = p.y > maxy ? p.y : maxy;
                    }
                    
                }
                Rect temp = Rect(minx, miny, maxx - minx, maxy - miny);
                log("i=%d x=%f,y=%f ,width=%f, height=%f",i,temp.origin.x,temp.origin.y, temp.size.width, temp.size.height);
                if (temp.intersectsRect(rect)) {
                    log("intersectsRect");
                }
            }
        
            log("detector");
        }else{
            log("no detector");
        }
         */
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
    
    //enemy
    
    //子弹
    const char * key = "effect_prt_020_1.plist";
    std::string file = FileUtils::getInstance()->fullPathForFilename(key);
    ParticleSystemQuad * effect = ParticleSystemQuad::create(file);
    this->addChild(effect);
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
            
           
            
        }
        
        for (int i=1; i<getUnits()->count(); i++) {
            //看看是否一直线
            if (m_currentTarget[i] == m_currentTarget[i-1]) {
                Armature * unit = dynamic_cast<Armature*>(this->getUnits()->getObjectAtIndex(i));
                Armature *lastUnit = dynamic_cast<Armature*>( getUnits()->getObjectAtIndex(i-1) );
                Point nowPos = unit->getPosition();
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
    
    
    //判断是否对敌开枪
    for (int i=0; i<getUnits()->count(); i++) {
        Point unitPos = dynamic_cast<Armature*>(getUnits()->getObjectAtIndex(i))->getPosition();
        Point enemyPos = enemy->getPosition();
        if (unitPos.getDistance(enemyPos) < 20) {
            const char * key = "effect_prt_020_1.plist";
            std::string file = FileUtils::getInstance()->fullPathForFilename(key);
            ParticleSystemQuad * effect = ParticleSystemQuad::create(file);
            effect->setPosition(enemyPos);
            this->addChild(effect);
        }
    }
    
}