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
    TMXTiledMap *map;
    TMXLayer* metaLayer;
    int maptrix[10][15] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,2,0,0,0,3,0,0,0,0,},
        {0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,},
        {0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
        {1,1,1,1,1,0,0,0,0,3,4,0,0,0,0,},
        {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
    };
    /*
     int maptrix[10][15] = {
     {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
     {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,},
     {0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,},
     {0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,},
     {0,0,0,0,1,0,0,0,0,0,1,1,0,0,0,},
     {1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,},
     {1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,},
     {1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,},
     {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,},
     {0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,}
     };

     */
    
public:
    static Scene *scene();
    virtual bool init();
    void update (float delta);
    bool collisionWithTile(Point pos);
    
    void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    void spriteMoveFinish(Node *sender);
    
    CREATE_FUNC(LevelLayer);
};


#endif /* defined(__CanyonDefense__LevelLayer__) */
