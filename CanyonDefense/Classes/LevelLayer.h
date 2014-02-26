//
//  LevelLayer.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#ifndef __CanyonDefense__LevelLayer__
#define __CanyonDefense__LevelLayer__

#include "cocos2d.h"

USING_NS_CC;

class LevelLayer : public Layer {
    
    
public:
    static Scene *scene();
    virtual bool init();
    bool collisionWithTile(Point pos);
    void addTower(Point pos, int towerTag);
    void spriteMoveFinish(Node *sender);
    
    CREATE_FUNC(LevelLayer);
};


#endif /* defined(__CanyonDefense__LevelLayer__) */
