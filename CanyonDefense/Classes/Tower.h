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
    Enemy* getClosestTarget();
    
	CC_SYNTHESIZE(int, _range,  Range);
    
    CC_SYNTHESIZE_RETAIN(Enemy*, _target, Target);
};


class MissileTurretTower: public Tower {
    
public:
    static MissileTurretTower* create();
    bool initWithFileAndRange(const char *pszFilename, int range);
    void fire(float dt);
    
};

#endif /* defined(__CanyonDefense__Tower__) */