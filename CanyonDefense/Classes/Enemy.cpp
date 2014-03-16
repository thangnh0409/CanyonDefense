//
//  Enemy.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "Enemy.h"
#include "Missile.h"
#include "GameMediator.h"
#include "MapManager.h"

enum{
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};

#define TITLE_WIDTH 32
#define TITLE_HEIGHT 32
#define kSpeedX         0.3
#define kSpeedY         0.3
#define kEnergySmallCar     6
#define kEnergyMediumCar    4
#define kEnergyBigCar       5
#define kResourceSmallCar   20
#define kResourceMediumCar  40
#define kResourceBigCar     100
/*
 construction method
 **/
Enemy::Enemy()
{
    log("Enemy: call init");
    velocity = 1 / 1; //480 fs/ second
    direction = RIGHT;
}
Enemy::~Enemy()
{
    
}
bool Enemy::initWithFile(const char *filename)
{
    bool bRet= false;
    do {
        sprite = Sprite::create(filename);
        sprite->setAnchorPoint(Point::ZERO);
        this->addChild(sprite);
        energySprite = Sprite::create("energy.png");
        energySprite->setPosition(this->getPosition() + Point(10, 25));
        this->addChild(energySprite);
        
        gm = GameMediator::shareInstance();
        //add schedule update enemy
        //schedule(schedule_selector(Enemy::enemyLogic), 0.1f);
        
        scheduleUpdate();
        
        bRet = true;
    } while (0);
    return bRet;
}

bool Enemy::initWithSpriteFrame(cocos2d::SpriteFrame *frame)
{
    bool bRet= false;
    do {
        sprite = Sprite::createWithSpriteFrame(frame);
        sprite->setAnchorPoint(Point::ZERO);
        this->addChild(sprite);
        energySprite = Sprite::create("energy.png");
        energySprite->setPosition(this->getPosition() + Point(10, 25));
        this->addChild(energySprite);
        
        gm = GameMediator::shareInstance();
        
        scheduleUpdate();
        
        bRet = true;
    } while (0);
    return bRet;

}
void Enemy::update(float dt)
{
    switch (direction) {
        case TOP:
            realSpeedX = 0;
            realSpeedY = kSpeedY;
            //this->setRotation(45);
            break;
        case BOTTOM:
            realSpeedX = 0;
            realSpeedY = -kSpeedY;
            break;
        case RIGHT:
            realSpeedX = kSpeedX;
            realSpeedY = 0;
            break;
        case LEFT:
            realSpeedX = -kSpeedX;
            realSpeedY = 0;
            break;
        default:
            break;
    }
    this->setPosition(getPositionX() + realSpeedX, getPositionY() + realSpeedY);
    
    Point pos = this->getPosition();
    //log("pos : x = %f, y = %f", pos.x, pos.y);
    int x = (pos.x) / 50;
    int y = ((10 * 32) - pos.y) / 32;
    //log("x = %d, y = %d", y, x);
    this->getNextDirection(maptrix, y, x);
    
    if (this->getEnergy() <= 0) {
        removeSelf();
    }else
        updateEnergy();

}
void Enemy::removeSelf()
{
    this->unscheduleUpdate();
    this->stopAllActions();
    this->removeFromParentAndCleanup(true);
    GameMediator::shareInstance()->getTargets()->removeObject(this);
}
void Enemy::updateEnergy()
{
    energySprite->setScaleX(this->getEnergy() / this->getMaxEnergy());
}
Rect Enemy::getRect()
{
    Size s = this->getContentSize();
    return Rect(this->getPositionX() - s.width/2, this->getPositionY() - s.height/2, s.width, s.height);
}

void Enemy::handleCollisionWithTile(bool collision)
{
    if(collision){
        switch (direction) {
            case RIGHT:
                direction = TOP;
                break;
            case TOP:
                direction = RIGHT;
                break;
            
            default:
                break;
        }
    }
}
void Enemy::getNextDirection(int matrix[10][15], int x, int y)
{
    if (matrix[x][y] != 0) {
        direction = matrix[x][y] - 1;
        switch (direction) {
            case TOP:
                sprite->setTexture("tank03_03.png");
                break;
            case RIGHT:
                sprite->setTexture("tank07_07.png");
                break;
            case LEFT:
                sprite->setTexture("tank05_05.png");
                break;
            case BOTTOM:
                sprite->setTexture("tank01_01.png");
                break;
            default:
                break; 
        }
    }
}

SmallCarEnemy* SmallCarEnemy::create()
{
    SmallCarEnemy* smallCar = new SmallCarEnemy();
    if (smallCar && smallCar->initWithFile("tank07_07.png")) {
        smallCar->autorelease();
        return smallCar;
    }
    CC_SAFE_DELETE(smallCar);
    return NULL;
}
bool SmallCarEnemy::initWithFile(const char *filename)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Enemy::initWithFile(filename));
        setEnergy(kEnergySmallCar);
        setMaxEnergy(kEnergySmallCar);
        setEnemyType(GROUND);
        bRet = true;
    } while (0);
    return bRet;
}

void SmallCarEnemy::update(float dt)
{
    switch (direction) {
        case TOP:
            realSpeedX = 0;
            realSpeedY = kSpeedY;
            //this->setRotation(45);
            break;
        case BOTTOM:
            realSpeedX = 0;
            realSpeedY = -kSpeedY;
            break;
        case RIGHT:
            realSpeedX = kSpeedX;
            realSpeedY = 0;
            break;
        case LEFT:
            realSpeedX = -kSpeedX;
            realSpeedY = 0;
            break;
        default:
            break;
    }
    this->setPosition(getPositionX() + realSpeedX, getPositionY() + realSpeedY);
    
    Point pos = this->getPosition();
    ///log("pos : x = %f, y = %f", pos.x, pos.y);
    int x = (pos.x) / 32;
    int y = ((10 * 32) - pos.y) / 32;
    //log("x = %d, y = %d", y, x);
    this->getNextDirection(maptrix, y, x);
    
    if (this->getEnergy() <= 0) {
        removeSelf();
        gm->getGameHUD()->updateResource(kResourceSmallCar);
    }else
        updateEnergy();
    if (pos.y <= 0) {
        removeSelf();
        gm->getGameHUD()->updateLive();
    }

}
MediumCarEnemy* MediumCarEnemy::create()
{
    MediumCarEnemy* mediumCar = new MediumCarEnemy();
    if (mediumCar && mediumCar->initWithFile("tank08_08.png")) {
        mediumCar->autorelease();
        return mediumCar;
    }
    CC_SAFE_DELETE(mediumCar);
    return NULL;
}
bool MediumCarEnemy::initWithFile(const char *filename)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Enemy::initWithFile(filename));
        setEnergy(kEnergyMediumCar);
        setMaxEnergy(kEnergyMediumCar);
        setEnemyType(GROUND);
        bRet = true;
    } while (0);
    return bRet;
}

BigCarEnemy* BigCarEnemy::create()
{
    BigCarEnemy* bigCar = new BigCarEnemy();
    if (bigCar && bigCar->initWithFile("tank06_06.png")) {
        bigCar->autorelease();
        return bigCar;
    }
    CC_SAFE_DELETE(bigCar);
    return NULL;
}
bool BigCarEnemy::initWithFile(const char *filename)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Enemy::initWithFile(filename));
        setEnergy(kEnergyBigCar);
        setMaxEnergy(kEnergyBigCar);
        setEnemyType(GROUND);
        bRet = true;
    } while (0);
    return bRet;
}

/**
 Rong boi duoi mat nuoc - loai nho
 */

SmallDragonEnemy* SmallDragonEnemy::create()
{
    SmallDragonEnemy* smallCar = new SmallDragonEnemy();
    if (smallCar && smallCar->initWithFile("dragon_1_1.png")) {
        smallCar->autorelease();
        return smallCar;
    }
    CC_SAFE_DELETE(smallCar);
    return NULL;
}
bool SmallDragonEnemy::initWithFile(const char *filename)
{
    bool bRet = false;
    do {
        CC_BREAK_IF(!Enemy::initWithFile(filename));
        setEnergy(kEnergySmallCar);
        setMaxEnergy(kEnergySmallCar);
        setEnemyType(GROUND);
        bRet = true;
        
        
        TMXTiledMap* map = MapManager::shareMap()->getTileMap();
        auto objectGroup = map->getObjectGroup("objects");
        auto& rootObj = objectGroup->getObjects();
        
        Value objectsVal = Value(rootObj);
        
        auto arrayPoint = PointArray::create(20);
        
        for (auto& obj : rootObj)
        {
            ValueMap& dict = obj.asValueMap();
            float x = dict["x"].asFloat();
            float y = dict["y"].asFloat();
            arrayPoint->addControlPoint(Point(x ,y));
        }
        
        this->setPosition(arrayPoint->getControlPointAtIndex(0));
        auto action = CatmullRomTo::create(20, arrayPoint);
        this->runAction(action);
        
        
    } while (0);
    return bRet;
}

void SmallDragonEnemy::update(float dt)
{
    Point pos = this->getPosition();
    
    if (this->getEnergy() <= 0) {
        removeSelf();
        gm->getGameHUD()->updateResource(kResourceSmallCar);
    }else
        updateEnergy();
    if (gm->getGameLayer()->isOutOfBound(pos)) {
        removeSelf();
        gm->getGameHUD()->updateLive();
    }
    
}

/**
@Rong bay tren troi - loai nho
*/


SmallFlyDragonEnemy* SmallFlyDragonEnemy::create()
{
    SmallFlyDragonEnemy* smallCar = new SmallFlyDragonEnemy();
    if (smallCar && smallCar->initWithFile("dragon_fly0.png")) {
        smallCar->autorelease();
        return smallCar;
    }
    CC_SAFE_DELETE(smallCar);
    return NULL;
}
bool SmallFlyDragonEnemy::initWithFile(const char *filename)
{
    bool bRet = false;
    do {
        auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
        
        float scale = Director::getInstance()->getContentScaleFactor();
        // manually add frames to the frame cache
        auto frame0 = SpriteFrame::createWithTexture(texture, Rect(128*0, 128*0, 128/scale, 128/scale));
        auto frame1 = SpriteFrame::createWithTexture(texture, Rect(128*1/scale, 128*0, 128/scale, 128/scale));
        auto frame2 = SpriteFrame::createWithTexture(texture, Rect(128*2/scale, 128*0, 128/scale, 128/scale));
        auto frame3 = SpriteFrame::createWithTexture(texture, Rect(128*3/scale, 128*0, 128/scale, 128/scale));
        auto frame4 = SpriteFrame::createWithTexture(texture, Rect(128*4/scale, 128*0, 128/scale, 128/scale));
        auto frame5 = SpriteFrame::createWithTexture(texture, Rect(128*5/scale, 128*0, 128/scale, 128/scale));
        auto frame6 = SpriteFrame::createWithTexture(texture, Rect(128*6/scale, 128*0, 128/scale, 128/scale));
        auto frame7 = SpriteFrame::createWithTexture(texture, Rect(128*7/scale, 128*0, 128/scale, 128/scale));
        
        CC_BREAK_IF(!Enemy::initWithSpriteFrame(frame0));
        
        setEnergy(kEnergySmallCar);
        setMaxEnergy(kEnergySmallCar);
        setEnemyType(AIR);
        velocity = 0.5 ;
        bRet = true;
        
        Size size = Director::getInstance()->getVisibleSize();
        int x = arc4random() % ((int)size.width - 100) + 100;
        log("x = %d", x);
        this->setPosition(Point(x, size.height));
    
        
        //
        // Animation using Sprite BatchNode
        
        sprite->setScale(0.3);
        
        Vector<SpriteFrame*> animFrames(10);
        animFrames.pushBack(frame0);
        animFrames.pushBack(frame1);
        animFrames.pushBack(frame2);
        animFrames.pushBack(frame3);
        animFrames.pushBack(frame4);
        animFrames.pushBack(frame5);
        animFrames.pushBack(frame6);
        animFrames.pushBack(frame7);
        //animFrames.pushBack(frame8);
        //animFrames.pushBack(frame9);
        
        auto animation = Animation::createWithSpriteFrames(animFrames,0.2f);
        auto animate = Animate::create(animation);
        sprite->runAction(RepeatForever::create( animate ) );
        

        
    } while (0);
    return bRet;
}

void SmallFlyDragonEnemy::update(float dt)
{
    this->setPosition(Point(this->getPositionX(), this->getPositionY() - velocity));
    Point pos = this->getPosition();
    
    if (this->getEnergy() <= 0) {
        removeSelf();
        gm->getGameHUD()->updateResource(kResourceSmallCar);
    }else
        updateEnergy();
    if (gm->getGameLayer()->isOutOfBound(pos)) {
        removeSelf();
        gm->getGameHUD()->updateLive();
    }
    
}
