//
//  Enemy.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "Enemy.h"
#include "Missile.h"

enum{
    TOP,
    RIGHT,
    BOTTOM,
    LEFT
};

#define TITLE_WIDTH 32
#define TITLE_HEIGHT 32
#define kSpeedX         0.5
#define kSpeedY         0.5
#define kEnergySmallCar     3
#define kEnergyMediumCar    4
#define kEnergyBigCar       5
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
        this->addChild(sprite);
        //add schedule update enemy
        //schedule(schedule_selector(Enemy::enemyLogic), 0.1f);
        
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
    int x = (pos.x) / 32;
    int y = ((10 * 32) - pos.y) / 32;
    //log("x = %d, y = %d", y, x);
    this->getNextDirection(maptrix, y, x);


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
        setEnemyType(GROUND);
        bRet = true;
    } while (0);
    return bRet;
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
        setEnemyType(GROUND);
        bRet = true;
    } while (0);
    return bRet;
}