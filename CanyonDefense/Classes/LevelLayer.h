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
    EASY = 1,
    NORMAL = 2,
    HARD = 3
    };
enum GAME_STATE {
    RUNNING = 0,
    PAUSE = 1,
    FINISH = 2,
    WAIT_NEXT_WAVE = 3
    };

class LevelLayer : public Layer {
    
private:
    SpriteFrameCache* cache;
    Vector<Node*> actionManager;
    Vector<Object*> schedulerManager;
    
    
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
     Call play game when you're wating for next wave
     */
    void playGameWhiteWaitNextWave();

    /**
     method for pause game
     */
    void callPauseGame();
    /**
     method for resume game
     */
    void callResumeGame();
    
    /**
     back to select difficult scene
     */
    void backScene();
    /**
     check finish wave
     */
    void checkFinishWave(float dt);
    /**
     detect out of bound Screen
     @param: pos: position check
     */
    bool isOutOfBound(Point pos);
    
    /**
     get timer delay by level map
     */
    float getTimerDelay(int levelMap);
    
    /**
     update game each frame
     */
    void update(float dt);
    
    void addExplosion(Point pos, int projectileType);
    
    void spriteMoveFinish(Node *sender);
    
    /*
     detect touch began
     **/
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    
    Point rootPoint;

    CC_SYNTHESIZE(int, _currentWaveCount, CurrentWaveCount);
    CC_SYNTHESIZE(int, _gameState, GameState);
    
    CREATE_FUNC(LevelLayer);
};


#endif /* defined(__CanyonDefense__LevelLayer__) */
