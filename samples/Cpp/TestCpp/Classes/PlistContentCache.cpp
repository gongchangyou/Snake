//
//  File.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-7-12.
//
//

#include "PlistContentCache.h"

PlistContentCache* m_PlistContentCacheInstance = NULL;

PlistContentCache::PlistContentCache(){
}

PlistContentCache::~PlistContentCache(){}
PlistContentCache * PlistContentCache::getInstance(){
    if (!m_PlistContentCacheInstance) {
        m_PlistContentCacheInstance = new PlistContentCache();
    }
    return m_PlistContentCacheInstance;
}