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
#include "Enemy.h"

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

/**
 Ten lua ban ra tu Catapult
 **/

class MissileProjectile : public Projectile {
public:
    bool initWithTarget(Enemy* enemy);
    static MissileProjectile* create(Enemy* enemy);
    void update(float dt);
    
private:
    
	float angularVelocity ;
    
	cocos2d::ParticleFire* m_emitter;
    
	float to360Angle(float angle)
	{
		angle = (int)angle % 360;
		if (angle < 0)
			angle += 360;
		return angle;
	}
    
	Enemy* myEnemy;
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

/**
 Dan xanh ban ra tu SacredOakTower
 */
class SacredOakProjectile : public Projectile {
private:
    Enemy* myEnemy;
public:
    bool initWithTarget(Enemy* enemy, Point selfPost);
    static SacredOakProjectile* create(Enemy* enemy, Point selfPost);
    void update(float dt);
    void moveToTargetPos();
    
};

/**
 Ten lua ban ra tu ThorTemple
 */
class ThorTempleProjectile : public Projectile {
private:
    Point targetPos;
public:
    bool initWithTargetPos(Point tgPos);
    static ThorTempleProjectile* create(Point tgPos);
    void createExplosion();
    
};

#endif /* defined(__CanyonDefense__Projectile__) */
