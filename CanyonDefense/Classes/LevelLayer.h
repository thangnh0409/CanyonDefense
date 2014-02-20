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
    
private:
    Array *enemyArray;
    Array *projectileArray;
    
public:
    static Scene *scene();
    virtual bool init();
    void update (float delta);
    
    void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    void spriteMoveFinish(Node *sender);
    
    CREATE_FUNC(LevelLayer);
};


#endif /* defined(__CanyonDefense__LevelLayer__) */
