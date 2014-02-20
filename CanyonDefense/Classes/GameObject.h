//
//  GameObject.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#ifndef __CanyonDefense__GameObject__
#define __CanyonDefense__GameObject__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class GameObject : public Sprite {
public:
    virtual void handleCollisionWith(GameObject* obj);
    virtual void update();
};


#endif /* defined(__CanyonDefense__GameObject__) */
