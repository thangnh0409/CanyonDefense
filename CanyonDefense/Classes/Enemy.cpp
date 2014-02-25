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
/*
 construction method
 **/
Enemy::Enemy()
{
    log("Enemy: call init");
    velocity = 1 / 1; //480 fs/ second
    direction = RIGHT;
}
Enemy* Enemy::create(const char *filename)
{
    Enemy *sprite = new Enemy();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}
Rect Enemy::getRect()
{
    Size s = this->getContentSize();
    return Rect(this->getPositionX() - s.width/2, this->getPositionY() - s.height/2, s.width, s.height);
}
void Enemy::handleCollisionWith(GameObject *obj)
{
    Missile *missile = dynamic_cast<Missile*>(obj);
    if (missile) {
        // occur collision
        log("occur collison2");
    }
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
    //log("matrix[%d][%d]= %d",x,y, matrix[1][0]);
    /*if (direction != BOTTOM) {
        if (matrix[x][y+1] == 1) {
            direction = RIGHT;
        }else
            if (matrix[x-1][y] == 1) {
                log("matrix = %d ", matrix[x][y-1]);
                direction = TOP;
            }else
                if (matrix[x+1][y] == 1) {
                    direction = BOTTOM;
                }
    }else
        direction = LEFT;*/
    if (matrix[x][y] != 0) {
        direction = matrix[x][y] - 1;
        switch (direction) {
            case TOP:
                this->setTexture("tank03_03.png");
                break;
            case RIGHT:
                this->setTexture("tank07_07.png");
                break;
            case LEFT:
                this->setTexture("tank05_05.png");
                break;
            case BOTTOM:
                this->setTexture("tank01_01.png");
                break;
            default:
                break; 
        }
    }
}
void Enemy::update()
{
    //this->runAction(MoveTo::create( TITLE_WIDTH / velocity, Point(getPositionX() + TITLE_WIDTH, getPositionY())));
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
}