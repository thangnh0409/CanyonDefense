//
//  Enemy.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "Enemy.h"
#include "Missile.h"

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

void Enemy::handleCollisionWith(GameObject *obj)
{
    Missile *missile = dynamic_cast<Missile*>(obj);
    if (missile) {
        // occur collision
        log("occur collison2");
    }
}
void Enemy::update()
{
    
}