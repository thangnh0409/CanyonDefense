//
//  Tower.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/24/14.
//
//

#ifndef __CanyonDefense__Tower__
#define __CanyonDefense__Tower__

#include "cocos2d.h"
#include "Enemy.h"

USING_NS_CC;

class Tower : public Node{
    
public:
    virtual ~Tower();
	virtual bool initWithFileAndRange(const char *pszFilename, int range);
    
    void towerLogic(float dt);
    Rect getRect();
    Enemy* getClosestTarget();
    
    Sprite* twSprite;
    Sprite* rangeTower;
    
	CC_SYNTHESIZE(int, _range,  Range);
    
    CC_SYNTHESIZE_RETAIN(Enemy*, _target, Target);
};


class MissileTurretTower: public Tower {
    
public:
    static MissileTurretTower* create();
    bool initWithFileAndRange(const char *pszFilename, int range);
    void fire(float dt);
    
};

class HutBasicTower: public Tower {
    
public:
    static HutBasicTower* create();
    bool initWithFileAndRange(const char *pszFilename, int range);
    void fire(float dt);
    
};

/**
 Sung ban ten lua
 */
class CatapultTower: public Tower {
    
public:
    static CatapultTower* create();
    bool initWithFileAndRange(const char *pszFilename, int range);
    void fire(float dt);
    
};

/*********************
 ADVANCE TOWER
 **********************/

class HutAdvanceTower: public Tower {
    
public:
    static HutAdvanceTower* create();
    bool initWithFileAndRange(const char *pszFilename, int range);
    void fire(float dt);
    
};

/**
 Sung cua than thanh
 @chuc nang: ban rong bay tren khong
 @dan: SacredOakProjectile
 */
class SacredOakTower: public Tower {
    
public:
    static SacredOakTower* create();
    bool initWithFileAndRange(const char *pszFilename, int range);
    void fire(float dt);
    
};

/*************************
 
 BUIDNG TOWER
 *************************/

/**
 Den tho Thor
 @chuc nang:ban ten lua hanh trinh
 **/

class ThorTempleTower: public Tower {
    
public:
    static ThorTempleTower* create();
    bool initWithFileAndRange(const char *pszFilename, int range);
    void addSkill(float dt);
    
};
#endif /* defined(__CanyonDefense__Tower__) */
