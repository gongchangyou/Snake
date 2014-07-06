//
//  MissionScene.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-30.
//
//

#ifndef __cocos2d_samples__MissionScene__
#define __cocos2d_samples__MissionScene__
#include "cocos2d.h"
#include <cocostudio/CocoStudio.h>
USING_NS_CC;
const int QUEUE_COUNT = 1;
using namespace cocostudio;
class MissionScene : public Layer{
public:
    CREATE_FUNC(MissionScene);
    MissionScene();
    virtual ~MissionScene();
    virtual bool init();
    static Scene* scene();
    virtual void update (float deltaTime);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
private:
    Point getVector(Point startPos, Point endPos);
    std::map<int, Point>m_paths;
    std::map<int, Point>m_vector;
    std::map<int, int>m_currentTarget;
    //CC_SYNTHESIZE_RETAIN(__Array*, m_paths, Paths);
    CC_SYNTHESIZE_RETAIN(__Array*, m_units, Units);
};

#endif /* defined(__cocos2d_samples__MissionScene__) */
