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
#include "Wave.h"

USING_NS_CC;

enum LEVEL_TYPE {
    EASY = 0,
    NORMAL = 1,
    HARD = 2
    };

class LevelLayer : public Layer {
    
    
public:
    static Scene *scene();
    virtual bool init();
    bool collisionWithTile(Point pos);
    
    /*
     method for level logic
     **/
    void levelLogic(float dt);
    /*
     method for add tower when player touch and drag tower into screen
     */
    void addTower(Point pos, int towerTag);
    
    /*
     method for add wave of enemy
     **/
    void addWave();
    
    /*
     get current wave
     **/
    Wave* getCurrentWave();
    /*
     get next wave in level
     **/
    void getNextWave();
    /*
     add enemy
     **/
    void addEnemy(float dt);
    
    /**
     detect out of bound Screen
     @param: pos: position check
     */
    bool isOutOfBound(Point pos);
    void spriteMoveFinish(Node *sender);
    
    /*
     detect touch began
     **/
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    
    Point rootPoint;
    /*
     variable for get level type
     **/
    CC_SYNTHESIZE(int, _levelType, LevelType);
    CC_SYNTHESIZE(int, _currentWaveCount, CurrentWaveCount);
    
    CREATE_FUNC(LevelLayer);
};


#endif /* defined(__CanyonDefense__LevelLayer__) */
