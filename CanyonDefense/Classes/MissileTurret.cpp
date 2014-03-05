//
//  MissileTurret.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "MissileTurret.h"

#define RADIUS_CAN_FIRE     40

MissileTurret* MissileTurret::create(const char *filename)
{
    MissileTurret *sprite = new MissileTurret();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        sprite->prepare();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    
    return NULL;
}
void MissileTurret::prepare()
{
    this->schedule(schedule_selector(MissileTurret::fire), 1.0f);
}
void MissileTurret::handleCollisionWith(GameObject* obj)
{
    
}
void MissileTurret::update()
{
    
}
bool MissileTurret::detectIntrusion(Point pos)
{
    Point myPos = this->getPosition();
    float distance = sqrtf((myPos.x - pos.x)*(myPos.x -pos.x) + (myPos.y - pos.y)*(myPos.y - pos.y));
    //log("distance = %f", distance);
    if (distance < RADIUS_CAN_FIRE) {
        return true;
    }
    return false;
}
void MissileTurret::fire(float dt)
{
    
}