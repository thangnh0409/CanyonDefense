//
//  Tower.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/24/14.
//
//

#include "Tower.h"
#include "GameMediator.h"
#include "Projectile.h"

Tower::~Tower()
{
    
}

bool Tower::initWithFileAndRange(const char *pszFilename, int range)
{
    bool bRet = false;
    do {
        // add somethings
        
        twSprite = Sprite::create(pszFilename);
        this->addChild(twSprite);
        
        this->setRange(range);
        _target = NULL;
        
        this->schedule(schedule_selector(Tower::towerLogic), 0.2f);
        bRet = true;
    } while (0);
    return bRet;
}
Enemy* Tower::getClosestTarget()
{
    Enemy* closestEnemy = NULL;
    double minDistance = 9999;
    Object* child;
    GameMediator* gm = GameMediator::shareInstance();
    CCARRAY_FOREACH(gm->getTargets(), child){
        Enemy* enemy = (Enemy*)child;
        double distance = ccpDistance(this->getPosition() , enemy->getPosition());
        if (distance < minDistance) {
            minDistance = distance;
            closestEnemy = enemy;
        }
    }
    if (minDistance < this->getRange()) {
        return closestEnemy;
    }
    return NULL;
}
void Tower::towerLogic(float dt)
{
    if (this->getTarget() == NULL) {
        this->setTarget(this->getClosestTarget());
    }else{
        double currDistance = this->getPosition().getDistance(this->getTarget()->getPosition());
        if (currDistance > this->getRange() || this->getTarget()->getEnergy() <= 0) {
            this->setTarget(this->getClosestTarget());
        }
    }
    if(this->getTarget() != NULL){
		Point shootVector = this->getTarget()->getPosition() - this->getPosition();
		float shootAngle = shootVector.getAngle();
		float cocosAngle = CC_RADIANS_TO_DEGREES(-1 * shootAngle);
        
		float rotateSpeed = (float)(0.25 / M_PI);
		float rotateDuration = fabs(shootAngle * rotateSpeed);
		//this->runAction(CCRotateTo::create(rotateDuration, cocosAngle));
	}

}
Rect Tower::getRect()
{
    Size s = twSprite->getContentSize();
    return Rect(this->getPositionX() - s.width/2, this->getPositionY() - s.height/2, s.width, s.height);
}

MissileTurretTower* MissileTurretTower::create()
{
    MissileTurretTower* mtt = new MissileTurretTower;
    if (mtt && mtt->initWithFileAndRange("CannonTurret.png", 100)) {
        mtt->autorelease();
        
        return mtt;
    }
    
    CC_SAFE_DELETE(mtt);
    return NULL;
}
bool MissileTurretTower::initWithFileAndRange(const char *pszFilename, int range)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        this->schedule(schedule_selector(MissileTurretTower::fire), 1.0f);
        bRet = true;
    } while (0);
    return bRet;
}
void MissileTurretTower::fire(float dt)
{
    if(this->getTarget() != NULL){
		GameMediator* m = GameMediator::shareInstance();
        
		Point shootVector = this->getTarget()->getPosition() - this->getPosition();
		Point normalizedShootVector = shootVector.normalize();
		Point overshotVector = normalizedShootVector * this->getRange();
		Point offscreenPoint = this->getPosition() + overshotVector;
        
		MissileProjectile* projectile = MissileProjectile::create(offscreenPoint, this->getPosition());
		m->getGameLayer()->addChild(projectile);
	}
}

// hut basic tower

HutBasicTower* HutBasicTower::create()
{
    HutBasicTower* mtt = new HutBasicTower;
    if (mtt && mtt->initWithFileAndRange("hut_2.png", 100)) {
        mtt->autorelease();
        
        return mtt;
    }
    
    CC_SAFE_DELETE(mtt);
    return NULL;
}
bool HutBasicTower::initWithFileAndRange(const char *pszFilename, int range)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        this->schedule(schedule_selector(HutBasicTower::fire), 1.0f);
        bRet = true;
    } while (0);
    return bRet;
}
void HutBasicTower::fire(float dt)
{
    if(this->getTarget() != NULL){
		GameMediator* m = GameMediator::shareInstance();
        
		Point shootVector = this->getTarget()->getPosition() - this->getPosition();
		Point normalizedShootVector = shootVector.normalize();
		Point overshotVector = normalizedShootVector * this->getRange();
		Point offscreenPoint = this->getPosition() + overshotVector;
        
		ArcheryProjectile* projectile = ArcheryProjectile::create(offscreenPoint, this->getPosition());
		m->getGameLayer()->addChild(projectile);
	}
}
