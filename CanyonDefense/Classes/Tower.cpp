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
        switch (range) {
            case 100:
                rangeTower = Sprite::create("range_small.png");
                break;
            case 150:
                rangeTower = Sprite::create("range_big.png");
                break;
            default:
                rangeTower = Sprite::create("range_small.png");
                break;
        }
        rangeTower->setVisible(false);
        this->addChild(rangeTower);
        this->setContentSize(twSprite->getContentSize());
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
        double distance = this->getPosition().getDistance(enemy->getPosition());
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
    if (mtt && mtt->initWithFileAndRange("CannonTurret.png", 150)) {
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
        
		//MissileProjectile* projectile = MissileProjectile::create(offscreenPoint, this->getPosition());
		//m->getGameLayer()->addChild(projectile);
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
    if(this->getTarget() != NULL && this->getTarget()->getEnemyType() == GROUND ){
		GameMediator* m = GameMediator::shareInstance();
        
		Point shootVector = this->getTarget()->getPosition() - this->getPosition();
		Point normalizedShootVector = shootVector.normalize();
		Point overshotVector = normalizedShootVector * this->getRange();
		Point offscreenPoint = this->getPosition() + overshotVector;
        
		ArcheryProjectile* projectile = ArcheryProjectile::create(offscreenPoint, this->getPosition() + Point(0, this->getContentSize().height/2));
		m->getGameLayer()->addChild(projectile);
	}
}

// hut advance tower

HutAdvanceTower* HutAdvanceTower::create()
{
    HutAdvanceTower* mtt = new HutAdvanceTower;
    if (mtt && mtt->initWithFileAndRange("hut_1.png", 100)) {
        mtt->autorelease();
        
        return mtt;
    }
    
    CC_SAFE_DELETE(mtt);
    return NULL;
}
bool HutAdvanceTower::initWithFileAndRange(const char *pszFilename, int range)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        this->schedule(schedule_selector(HutAdvanceTower::fire), 1.0f);
        bRet = true;
    } while (0);
    return bRet;
}
void HutAdvanceTower::fire(float dt)
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

/**
Sung ban ten lua
*/

CatapultTower* CatapultTower::create()
{
    CatapultTower* mtt = new CatapultTower;
    if (mtt && mtt->initWithFileAndRange("ballias_3.png", 150)) {
        mtt->autorelease();
        
        return mtt;
    }
    
    CC_SAFE_DELETE(mtt);
    return NULL;
}
bool CatapultTower::initWithFileAndRange(const char *pszFilename, int range)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        this->schedule(schedule_selector(CatapultTower::fire), 1.0f);
        bRet = true;
    } while (0);
    return bRet;
}
void CatapultTower::fire(float dt)
{
    if(this->getTarget() != NULL){
		GameMediator* m = GameMediator::shareInstance();        
		MissileProjectile* projectile = MissileProjectile::create(this->getTarget());
        projectile->setPosition(this->getPosition());
		projectile->setRotation(this->getRotation());
		m->getGameLayer()->addChild(projectile);
	}
}


/**
 Sung ban rong bay tren khong
 */

SacredOakTower* SacredOakTower::create()
{
    SacredOakTower* mtt = new SacredOakTower;
    if (mtt && mtt->initWithFileAndRange("sacred_oak_1.png", 150)) {
        mtt->autorelease();
        
        return mtt;
    }
    
    CC_SAFE_DELETE(mtt);
    return NULL;
}
bool SacredOakTower::initWithFileAndRange(const char *pszFilename, int range)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        this->schedule(schedule_selector(SacredOakTower::fire), 1.0f);
        bRet = true;
    } while (0);
    return bRet;
}
void SacredOakTower::fire(float dt)
{
    if(this->getTarget() != NULL && this->getTarget()->getEnemyType() == AIR){
		GameMediator* m = GameMediator::shareInstance();
		SacredOakProjectile* projectile = SacredOakProjectile::create(this->getTarget(), this->getPosition());
        projectile->setPosition(this->getPosition());
//        ThorTempleProjectile* projectile = ThorTempleProjectile::create(Point(50, 100), this->getPosition());
		m->getGameLayer()->addChild(projectile);
	}else
        this->getClosestTarget();
}

/**
 Vu khi xay dung, ban ten lua
 */

ThorTempleTower* ThorTempleTower::create()
{
    ThorTempleTower* mtt = new ThorTempleTower;
    if (mtt && mtt->initWithFileAndRange("temple_of_thor.png", 150)) {
        mtt->autorelease();
        
        return mtt;
    }
    
    CC_SAFE_DELETE(mtt);
    return NULL;
}
bool ThorTempleTower::initWithFileAndRange(const char *pszFilename, int range)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Tower::initWithFileAndRange(pszFilename, range));
        ParticleSystemQuad* _emitter = ParticleGalaxy::create();
        //_emitter->setTexture( Director::getInstance()->getTextureCache()->addImage("fire.png") );
        _emitter->setPosition(this->getPosition()+ Point(5, 20));
        _emitter->setScale(0.3);
        this->addChild(_emitter);
        
        this->schedule(schedule_selector(ThorTempleTower::addSkill), 10.0f);
        bRet = true;
    } while (0);
    return bRet;
}
void ThorTempleTower::addSkill(float dt)
{
    GameHUD::shareInstance()->setThorSkillAvailble(true);
}