//
//  GameScene.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#ifndef __CanyonDefense__GameScene__
#define __CanyonDefense__GameScene__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public Layer {
    
public:
    static Scene* createScene();
    virtual void onEnter();
    
    CREATE_FUNC(GameScene);
};



#endif /* defined(__CanyonDefense__GameScene__) */
