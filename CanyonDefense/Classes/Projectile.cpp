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
    this->setSpeed(480);
    return true;
}

MissileProjectile* MissileProjectile::create(Point targetPos)
{
    MissileProjectile* mp = new MissileProjectile();
    if (mp && mp->initWithTargetPos(targetPos)) {
        mp->autorelease();
        return mp;
    }
    CC_SAFE_DELETE(mp);
    return NULL;
}
bool MissileProjectile::initWithTargetPos(cocos2d::Point pos)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Projectile::initWithFileName("bullet1.png"));
        setTargetPos(pos);
        moveToTargetPos();
        
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
           // log("xay ra va cham!");
        }
    }
}
void MissileProjectile::moveToTargetPos()
{
    float distance = ccpDistance(this->getPosition(), this->getTargetPos());
	float moveDur = distance / getSpeed();
	auto moveTo = CCMoveTo::create(moveDur, this->getTargetPos());
	//auto moveDone = CCCallFunc::create(this, callfunc_selector(MachineProjectTile::removeSelf));
	this->runAction(CCSequence::create(moveTo, NULL));
}