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
#include "SimpleAudioEngine.h"

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

#define kSmallDragonEnergy              3
#define kMediumDragonEnergy             7
#define kBigDragonEnergy                15

#define kSmallFlyDragonEnergy           5
#define kMediumFlyDragonEnergy          10
#define kBigFlyDragonEnergy             15

#define kResourceSmallCar               20
#define kResourceMediumCar              40
#define kResourceBigCar                 100

#define kSmallDragonResource            20
#define kMediumDragonResource           40
#define kBigDragonResource              50
#define kSmallFlyDragonResource         40
#define kMediumFlyDragonResource        60
#define kBigFlyDragonResource           80
/*
 construction method
 **/
Enemy::Enemy()
{
    log("Enemy: call init");
    velocity = 1 / 1; //480 fs/ second
    direction = RIGHT;
    //array point for get path
    
    TMXTiledMap* map = MapManager::shareMap()->getTileMap();
    auto objectGroup = map->getObjectGroup("path1");
    auto& rootObj = objectGroup->getObjects();
    auto objectGroup2 = map->getObjectGroup("path2");
    auto& rootObj2 = objectGroup2->getObjects();
    
    arrayPoint = PointArray::create(20);
    arrayPoint2 = PointArray::create(20);
    
    for (auto& obj : rootObj)
    {
        ValueMap& dict = obj.asValueMap();
        float x = dict["x"].asFloat();
        float y = dict["y"].asFloat();
        arrayPoint->addControlPoint(Point(x ,y));
    }
    for (auto& obj : rootObj2)
    {
        ValueMap& dict = obj.asValueMap();
        float x = dict["x"].asFloat();
        float y = dict["y"].asFloat();
        arrayPoint2->addControlPoint(Point(x ,y));
    }

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
 Rong boi tren mat nuoc - loai nho
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
        setEnergy(kSmallDragonEnergy);
        setMaxEnergy(kSmallDragonEnergy);
        setEnemyType(GROUND);
        bRet = true;
        
        switch (arc4random()%2) {
            case 0:{
                this->setPosition(arrayPoint->getControlPointAtIndex(0));
                auto action = CatmullRomTo::create(20, arrayPoint);
                this->runAction(action);
                break;
            }
            case 1:{
                this->setPosition(arrayPoint2->getControlPointAtIndex(0));
                auto action = CatmullRomTo::create(20, arrayPoint2);
                this->runAction(action);
                break;
            }
            default:
                break;
        }
        
        
    } while (0);
    return bRet;
}

void SmallDragonEnemy::update(float dt)
{
    Point pos = this->getPosition();
    
    if (this->getEnergy() <= 0) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/tieng_dragon_2.wav");
        removeSelf();
        gm->getGameHUD()->setNumDrgIsKilled(gm->getGameHUD()->getNumDrgIsKilled() + 1);
        gm->getGameHUD()->updateResource(kSmallDragonResource);
    }else
        updateEnergy();
    if (gm->getGameLayer()->isOutOfBound(pos)) {
        removeSelf();
        gm->getGameHUD()->updateLive();
    }
    
}

/**
 Rong boi tren mat nuoc - loai vua
 */

MediumDragonEnemy* MediumDragonEnemy::create()
{
    MediumDragonEnemy* mediumDragon = new MediumDragonEnemy();
    if (mediumDragon && mediumDragon->initWithFile("dragon_water3.png")) {
        mediumDragon->autorelease();
        return mediumDragon;
    }
    CC_SAFE_DELETE(mediumDragon);
    return NULL;
}
bool MediumDragonEnemy::initWithFile(const char *filename)
{
    bool bRet = false;
    do {
        auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
        Vector<SpriteFrame*> animFrames(10);
        float scale = Director::getInstance()->getContentScaleFactor();
        // manually add frames to the frame cache
        for (int i=0; i < 7; i++){
            auto frame = SpriteFrame::createWithTexture(texture, Rect(128*i/scale, 128*4/scale, 128/scale, 128/scale));
            animFrames.pushBack(frame);
        }
        
        CC_BREAK_IF(!Enemy::initWithSpriteFrame(animFrames.front()));
        setEnergy(kMediumDragonEnergy);
        setMaxEnergy(kMediumDragonEnergy);
        setEnemyType(GROUND);
        bRet = true;
        
        sprite->setScale(0.3);
        auto animation = Animation::createWithSpriteFrames(animFrames,0.2f);
        auto animate = Animate::create(animation);
        sprite->runAction(RepeatForever::create( animate ) );
                
        switch (arc4random()%2) {
            case 0:{
                this->setPosition(arrayPoint->getControlPointAtIndex(0));
                auto action = CatmullRomTo::create(20, arrayPoint);
                this->runAction(action);
                break;
            }
            case 1:{
                this->setPosition(arrayPoint2->getControlPointAtIndex(0));
                auto action = CatmullRomTo::create(20, arrayPoint2);
                this->runAction(action);
                break;
            }
            default:
                break;
        }
        
        
    } while (0);
    return bRet;
}

void MediumDragonEnemy::update(float dt)
{
    Point pos = this->getPosition();
    
    if (this->getEnergy() <= 0) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/tieng_dragon_3.wav");
        removeSelf();
        gm->getGameHUD()->setNumDrgIsKilled(gm->getGameHUD()->getNumDrgIsKilled() + 1);
        gm->getGameHUD()->updateResource(kMediumDragonResource);
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
        Vector<SpriteFrame*> animFrames(10);
        float scale = Director::getInstance()->getContentScaleFactor();
        // manually add frames to the frame cache
        for (int i=0; i < 8; i++){
            auto frame = SpriteFrame::createWithTexture(texture, Rect(128*i/scale, 128*0, 128/scale, 128/scale));
            animFrames.pushBack(frame);
        }
        
        CC_BREAK_IF(!Enemy::initWithSpriteFrame(animFrames.front()));
        
        setEnergy(kSmallFlyDragonEnergy);
        setMaxEnergy(kSmallFlyDragonEnergy);
        setEnemyType(AIR);
        velocity = 0.3 ;
        bRet = true;
        
        Size size = Director::getInstance()->getVisibleSize();
        int x = (arc4random() % 500) + 200;
        log("x = %d", x);
        this->setPosition(Point(x, size.height));
    
        
        //
        // Animation using Sprite BatchNode
        
        sprite->setScale(0.4);
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
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/tieng_dragon_1.wav");
        removeSelf();
        gm->getGameHUD()->setNumDrgIsKilled(gm->getGameHUD()->getNumDrgIsKilled() + 1);
        gm->getGameHUD()->updateResource(kSmallFlyDragonResource);
    }else
        updateEnergy();
    if (gm->getGameLayer()->isOutOfBound(pos)) {
        removeSelf();
        gm->getGameHUD()->updateLive();
    }
    
}

/**
 @Rong bay tren troi - loai vua
 */


MediumFlyDragonEnemy* MediumFlyDragonEnemy::create()
{
    MediumFlyDragonEnemy* mediumFlyDragon = new MediumFlyDragonEnemy();
    if (mediumFlyDragon && mediumFlyDragon->initWithFile("dragon_fly4.png")) {
        mediumFlyDragon->autorelease();
        return mediumFlyDragon;
    }
    CC_SAFE_DELETE(mediumFlyDragon);
    return NULL;
}
bool MediumFlyDragonEnemy::initWithFile(const char *filename)
{
    bool bRet = false;
    do {
        auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
        Vector<SpriteFrame*> animFrames(10);
        float scale = Director::getInstance()->getContentScaleFactor();
        // manually add frames to the frame cache
        for (int i=0; i < 4; i++){
            auto frame = SpriteFrame::createWithTexture(texture, Rect(128*i/scale, 128*0, 128/scale, 128/scale));
            animFrames.pushBack(frame);
        }
        
        CC_BREAK_IF(!Enemy::initWithSpriteFrame(animFrames.front()));
        
        setEnergy(kMediumFlyDragonEnergy);
        setMaxEnergy(kMediumFlyDragonEnergy);
        setEnemyType(AIR);
        velocity = 0.5 ;
        bRet = true;
        
        Size size = Director::getInstance()->getVisibleSize();
        int x = (arc4random() % 500) + 100;
        this->setPosition(Point(x, size.height));
        
        
        //
        // Animation using Sprite BatchNode
        
        sprite->setScale(0.5);
        auto animation = Animation::createWithSpriteFrames(animFrames,0.1f);
        auto animate = Animate::create(animation);
        sprite->runAction(RepeatForever::create( animate ) );
        
        
        
    } while (0);
    return bRet;
}

void MediumFlyDragonEnemy::update(float dt)
{
    this->setPosition(Point(this->getPositionX(), this->getPositionY() - velocity));
    Point pos = this->getPosition();
    
    if (this->getEnergy() <= 0) {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/tieng_dragon_4.wav");
        removeSelf();
        gm->getGameHUD()->setNumDrgIsKilled(gm->getGameHUD()->getNumDrgIsKilled() + 1);
        gm->getGameHUD()->updateResource(kMediumFlyDragonResource);
    }else
        updateEnergy();
    if (gm->getGameLayer()->isOutOfBound(pos)) {
        removeSelf();
        gm->getGameHUD()->updateLive();
    }
    
}

