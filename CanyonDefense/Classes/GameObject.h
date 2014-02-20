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
private:
    bool isScheduledForRemove_ = false;
public:
    virtual void handleCollisionWith(GameObject* obj);
    virtual void update();
    void setIsScheduledForRemove(bool value){
        isScheduledForRemove_ = value;
        log("set obj for remove");
    };
    bool getScheduledForRemove() {
        return isScheduledForRemove_;
    }
};


#endif /* defined(__CanyonDefense__GameObject__) */
