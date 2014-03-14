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
    
    Sprite* mySprite;
    
    virtual bool initWithFileName(const char* filename);
    virtual Rect getRect();
    void removeSelf();
    void projectileMoveFinish(Node* sender);
    
    CC_SYNTHESIZE(float, _speed, Speed);
    CC_SYNTHESIZE(Point, _targetPos, TargetPos);
    CC_SYNTHESIZE(int, damage, Damage);

    

};

class MissileProjectile : public Projectile {
public:
    bool initWithTargetPos(Point pos, Point selfPos);
    static MissileProjectile* create(Point targetPos, Point selfPos);
    void update(float dt);
    void moveToTargetPos();
    
};

/**
 Mui ten ban ra tu Hut Basic
 */
class ArcheryProjectile : public Projectile {
public:
    bool initWithTargetPos(Point pos, Point selfPos);
    static ArcheryProjectile* create(Point targetPos, Point selfPos);
    void update(float dt);
    void moveToTargetPos();
    
};

#endif /* defined(__CanyonDefense__Projectile__) */
