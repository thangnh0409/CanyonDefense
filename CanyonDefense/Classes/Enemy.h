//
//  Enemy.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#ifndef __CanyonDefense__Enemy__
#define __CanyonDefense__Enemy__

#include "cocos2d.h"

USING_NS_CC;

class Enemy : public Node {
private:
    /*
     speedX of enemy
     **/
    float realSpeedX;
    /*
     speed Y of enemy
     **/
    float realSpeedY;
    /*
     variable for velocity of enemy
     **/
    float velocity;
    /*
     variable for direction of enemy
     **/
    int direction;
    int maptrix[10][15] = {
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,2,0,0,0,3,0,0,0,0,},
        {0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,},
        {0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
        {1,1,1,1,1,0,0,0,0,3,4,0,0,0,0,},
        {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
    };
    Sprite* sprite;
    
public:
    Enemy();
    virtual ~Enemy();
    virtual bool initWithFile(const char* filename);
    void update(float dt);
    
    Rect getRect();
    void handleCollisionWithTile(bool collision);
    void getNextDirection(int matrix[10][15], int x, int y);
};

/*
 Class for small car enemy
 */
class SmallCarEnemy : public Enemy {
public:
    static SmallCarEnemy* create();
    bool initWithFile(const char* filename);
    
};

/**
 Medium car enemy
 **/

class MediumCarEnemy : public Enemy {
    
public:
    static MediumCarEnemy* create();
    bool initWithFile(const char* filename);
};

/**
 Big car enemy
 **/

class BigCarEnemy : public Enemy {
    
public:
    static BigCarEnemy* create();
    bool initWithFile(const char* filename);
};
#endif /* defined(__CanyonDefense__Enemy__) */
