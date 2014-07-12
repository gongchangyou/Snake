//
//  File.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-7-12.
//
//

#ifndef __cocos2d_samples__File__
#define __cocos2d_samples__File__

#include "cocos2d.h"
USING_NS_CC;
class PlistContentCache : public __Dictionary{
public:
    PlistContentCache();
    ~PlistContentCache();
    static PlistContentCache * getInstance();
};
#endif /* defined(__cocos2d_samples__File__) */
