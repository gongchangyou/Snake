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
using namespace cocostudio;
class MissionScene : public Layer{
public:
    CREATE_FUNC(MissionScene);
    MissionScene();
    virtual ~MissionScene();
    virtual bool init();
    static Scene* scene();
private:
    

};

#endif /* defined(__cocos2d_samples__MissionScene__) */
