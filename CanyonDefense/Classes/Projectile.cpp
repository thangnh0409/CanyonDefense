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
MissileProjectile* MissileProjectile::create(Point targetPos, Point selfPos)
{
    MissileProjectile* mp = new MissileProjectile();
    if (mp && mp->initWithTargetPos(targetPos, selfPos)) {
        mp->autorelease();
        return mp;
    }
    CC_SAFE_DELETE(mp);
    return NULL;
}
bool MissileProjectile::initWithTargetPos(cocos2d::Point pos, Point selfPos)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Projectile::initWithFileName("bullet1.png"));
        this->setPosition(selfPos);
        setTargetPos(pos);
        moveToTargetPos();
        this->setDamage(1);
        scheduleUpdate();
        
        bRet = true;
    } while (0);
    return bRet;
}
Rect Projectile::getRect()
{
    Size s = mySprite->getContentSize();
    return Rect(this->getPositionX() - s.width/2, this->getPositionY() - s.height/2, s.width, s.height);
}
void MissileProjectile::update(float dt)
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
void MissileProjectile::moveToTargetPos()
{
    Size winSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    // Determinie offset of location to projectile
	float offX = getTargetPos().x - this->getPosition().x;
	float offY = getTargetPos().y - this->getPosition().y;
    //log("targetX=%f , thiX = %f, offX = %f, offY = %f", getTargetPos().x, this->getPositionX(), offX, offY);
    float tmp = offX;
    
	// Determine where we wish to shoot the projectile to
    float realX= origin.x+winSize.width + (mySprite->getContentSize().width);

	float ratio = (offY / fabs(offX));
	float realY = (realX * ratio) + this->getPositionY();
    if (tmp < 0) {
        realX = - (origin.x + mySprite->getContentSize().width/2);
    }
    
	Point realDest = Point(realX, realY);
    //log("real X = %f, realY=%f",realX, realY);
    
	// Determine the length of how far we're shooting
	float offRealX = realX - this->getPosition().x;
	float offRealY = realY - this->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
    //float distance = ccpDistance(this->getPosition(), this->getTargetPos());
	float realMoveDuration = length/ getSpeed();
	auto moveTo = CCMoveTo::create(realMoveDuration, realDest);
	//auto moveDone = CCCallFunc::create(this, callfunc_selector(MachineProjectTile::removeSelf));
	this->runAction(CCSequence::create(moveTo, CallFuncN::create( CC_CALLBACK_1(Projectile::projectileMoveFinish, this)), NULL));
}