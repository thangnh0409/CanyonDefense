//
//  Enemy.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#ifndef __CanyonDefense__Enemy__
#define __CanyonDefense__Enemy__

#include "GameObject.h"

class Enemy : public GameObject {
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
    
public:
    Enemy();
    static Enemy * create(const char *filename);
    virtual void handleCollisionWith(GameObject *obj);
    virtual void update();
    void handleCollisionWithTile(bool collision);
    void getNextDirection(int matrix[10][15], int x, int y);
};

#endif /* defined(__CanyonDefense__Enemy__) */
