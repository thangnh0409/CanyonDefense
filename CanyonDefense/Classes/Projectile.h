//
//  Projectile.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/24/14.
//
//

#ifndef __CanyonDefense__Projectile__
#define __CanyonDefense__Projectile__

#include "cocos2d.h"

USING_NS_CC;

class Projectile : public Node {
    
public:
    virtual bool initWithFileName(const char* filename);
    virtual Rect getRect();
    
    CC_SYNTHESIZE(float, _speed, Speed);
    CC_SYNTHESIZE(Point, _targetPos, TargetPos);
private:
    Sprite* mySprite;

};

class MissileProjectile : public Projectile {
public:
    bool initWithTargetPos(Point pos);
    static MissileProjectile* create(Point targetPos);
    void update(float dt);
    void moveToTargetPos();
};

#endif /* defined(__CanyonDefense__Projectile__) */
