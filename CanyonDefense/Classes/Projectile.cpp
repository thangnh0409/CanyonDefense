//
//  Projectile.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/24/14.
//
//

#include "Projectile.h"
#include "GameMediator.h"
#include "Enemy.h"
#include "SimpleAudioEngine.h"

#define kArcheryProjectileDamage            2
#define kMissileProjectileDamage            3
#define kSacredOakProjectileDamage          4
#define kThorTempleProjectileDamage         10

bool Projectile::initWithFileName(const char* filename)
{
    mySprite = Sprite::create(filename);
    this->addChild(mySprite);
    this->setSpeed(300);
    return true;
}

void Projectile::removeSelf()
{
    this->unscheduleUpdate();
    this->stopAllActions();
    this->removeFromParentAndCleanup(true);
}
void Projectile::projectileMoveFinish(cocos2d::Node *sender)
{
    log("remove self");
    removeSelf();
}

Rect Projectile::getRect()
{
    Size s = mySprite->getContentSize();
    return Rect(this->getPositionX() - s.width/2, this->getPositionY() - s.height/2, s.width, s.height);
}

MissileProjectile* MissileProjectile::create(Enemy* enemy)
{
    MissileProjectile* mp = new MissileProjectile();
    if (mp && mp->initWithTarget(enemy)) {
        mp->autorelease();
        return mp;
    }
    CC_SAFE_DELETE(mp);
    return NULL;
}
bool MissileProjectile::initWithTarget(Enemy* enemy)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Projectile::initWithFileName("rocket.png"));
        myEnemy = enemy;
        this->setDamage(kMissileProjectileDamage);
		this->setSpeed(120);
        
		angularVelocity = 5.0f;
        scheduleUpdate();
        
        bRet = true;
    } while (0);
    return bRet;
}

void MissileProjectile::update(float dt)
{
    GameMediator* gm = GameMediator::shareInstance();
    Point targetPos = myEnemy->getPosition();
	Point myPos = this->getPosition();
    
	if (gm->getGameLayer()->isOutOfBound(myPos) || myEnemy->getEnergy() <=0){
		removeSelf();
	}
    
	if(this->getRect().intersectsRect(myEnemy->getRect()) && myEnemy->getEnergy() > 0){
		myEnemy->setEnergy(myEnemy->getEnergy() - this->getDamage());
        gm->getGameLayer()->addExplosion(myEnemy->getPosition(), 1);
        
		removeSelf();
	}
    
	float radian = atan2f(targetPos.y - myPos.y, targetPos.x - myPos.x);
	float angle = -CC_RADIANS_TO_DEGREES(radian);
	angle = to360Angle(angle);
    
	float myAngle = to360Angle(this->getRotation());
	float tempAngle;
	if(myAngle < angle){
		tempAngle = angularVelocity;
	}else if(myAngle >angle){
		tempAngle = -angularVelocity;
	}else{
		tempAngle = 0;
	}
    
	if(360 - abs(angle - myAngle) < abs(angle - myAngle)){
		tempAngle *= -1;
	}
    
	this->setRotation(this->getRotation() + tempAngle);
	float myRadian = CC_DEGREES_TO_RADIANS(this->getRotation());
    
	float x = cosf(myRadian) * this->getSpeed() * dt;
	float y = -sinf(myRadian) * this->getSpeed() * dt;
    
	this->setPosition(Point(myPos.x + x, myPos.y + y));
}

/**
 Mui ten ban ra tu Hut Basic
 */
 
ArcheryProjectile* ArcheryProjectile::create(Point targetPos, Point selfPos)
{
    ArcheryProjectile* mp = new ArcheryProjectile();
    if (mp && mp->initWithTargetPos(targetPos, selfPos)) {
        mp->autorelease();
        return mp;
    }
    CC_SAFE_DELETE(mp);
    return NULL;
}
bool ArcheryProjectile::initWithTargetPos(cocos2d::Point pos, Point selfPos)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Projectile::initWithFileName("mui_ten_1.png"));
        this->setPosition(selfPos);
        setTargetPos(pos);
        moveToTargetPos();
        this->setDamage(kArcheryProjectileDamage);
        scheduleUpdate();
        
        bRet = true;
    } while (0);
    return bRet;
}

void ArcheryProjectile::update(float dt)
{
    GameMediator* gm = GameMediator::shareInstance();
    Object* child = NULL;
    CCARRAY_FOREACH(gm->getTargets(), child){
        Enemy* enemy = (Enemy*)child;
        if (this->getRect().intersectsRect(enemy->getRect())) {
            enemy->setEnergy(enemy->getEnergy() - this->getDamage());
            removeSelf();
        }
    }
}
void ArcheryProjectile::moveToTargetPos()
{
    Point shootVector = this->getTargetPos() - this->getPosition();
    float shootAngle = shootVector.getAngle();
    float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
    this->runAction(CCRotateTo::create(0, cocosAngle));
    
    float length = this->getTargetPos().getDistance(this->getPosition());
	float realMoveDuration = length/ getSpeed();
	auto moveTo = CCMoveTo::create(realMoveDuration, this->getTargetPos());
	this->runAction(CCSequence::create(moveTo, CallFuncN::create( CC_CALLBACK_1(Projectile::projectileMoveFinish, this)), NULL));
}

/**
 Dan ban ra tu SacredOakTower - Ban rong bay
*/

SacredOakProjectile* SacredOakProjectile::create(Enemy* enemy, Point selfPos)
{
    SacredOakProjectile* mp = new SacredOakProjectile();
    if (mp && mp->initWithTarget(enemy, selfPos)) {
        mp->autorelease();
        return mp;
    }
    CC_SAFE_DELETE(mp);
    return NULL;
}
bool SacredOakProjectile::initWithTarget(Enemy* enemy, Point selfPos)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Projectile::initWithFileName("sacred_oak_projectile.png"));
        myEnemy = enemy;
        this->setPosition(selfPos);
        this->setDamage(kSacredOakProjectileDamage);
		this->setSpeed(300);
        this->moveToTargetPos();
        scheduleUpdate();
        
        bRet = true;
    } while (0);
    return bRet;
}
void SacredOakProjectile::moveToTargetPos()
{
    float distance = this->getPosition().getDistance(myEnemy->getPosition());
    float duration = distance / this->getSpeed();
    auto actionMove = MoveTo::create(duration , myEnemy->getPosition());
    this->runAction(Sequence::create(actionMove, CallFuncN::create(CC_CALLBACK_1(Projectile::projectileMoveFinish, this)), NULL));
}
void SacredOakProjectile::update(float dt)
{
    GameMediator* gm = GameMediator::shareInstance();
    Point targetPos = myEnemy->getPosition();
	Point myPos = this->getPosition();
    
	if (gm->getGameLayer()->isOutOfBound(myPos) || myEnemy->getEnergy() <=0){
		removeSelf();
	}
    
	if(this->getRect().intersectsRect(myEnemy->getRect()) && myEnemy->getEnergy() > 0){
		myEnemy->setEnergy(myEnemy->getEnergy() - this->getDamage());
		gm->getGameLayer()->addExplosion(myEnemy->getPosition(), 0);
		removeSelf();
	}
}

/**
 Ten lua ban ra tu ThorTemple
 */

ThorTempleProjectile* ThorTempleProjectile::create(Point tgPos)
{
    ThorTempleProjectile* mp = new ThorTempleProjectile();
    if (mp && mp->initWithTargetPos(tgPos)) {
        mp->autorelease();
        return mp;
    }
    CC_SAFE_DELETE(mp);
    return NULL;
}
bool ThorTempleProjectile::initWithTargetPos(cocos2d::Point tgPos)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Projectile::initWithFileName("range.png"));

        this->setDamage(kThorTempleProjectileDamage);
        this->setTargetPos(tgPos);
        this->setPosition(tgPos);
        
        bRet = true;
    } while (0);
    return bRet;
}
void ThorTempleProjectile::createExplosion()
{
    GameMediator* gm = GameMediator::shareInstance();
    gm->getGameLayer()->addExplosion(getTargetPos(), 2);
    Object* child = NULL;
    CCARRAY_FOREACH(gm->getTargets(), child){
        Enemy* enemy = (Enemy*)child;
        log("RECT: width = %f, h = %f", this->getRect().size.width, this->getRect().size.height);
        if (this->getRect().intersectsRect(enemy->getRect())) {
            log("intersect");
            enemy->setEnergy(enemy->getEnergy() - this->getDamage());
            
        }
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/ten_lua.wav");
        removeSelf();
    }
    

}
