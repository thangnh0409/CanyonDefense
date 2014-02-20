//
//  Missile.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "Missile.h"
#include "Enemy.h"


Missile* Missile::create(const char *filename)
{
    Missile *sprite = new Missile();
    if (sprite && sprite->initWithFile(filename))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    
    return NULL;
}

void Missile::handleCollisionWith(GameObject* obj)
{
    Enemy *enemy = dynamic_cast<Enemy*>(obj);
    if (enemy) {
        // occur collision
        log("occur collision");
        Vector<SpriteFrame*> aniFrame(6);
        SpriteFrameCache* cache = SpriteFrameCache::getInstance();
        cache->addSpriteFramesWithFile("yulei1.plist", "yulei1.png");
        
        char str[100] = {0};
        for (int i = 1; i < 6; i++) {
            sprintf(str, "yulei1_0%d.png", i);
            auto frame = cache->getSpriteFrameByName(str);
            aniFrame.pushBack(frame);
        }
        Animation* animation = Animation::createWithSpriteFrames(aniFrame, 0.3f);
        enemy->runAction(Animate::create(animation));
        //enemy->removeFromParent();
        
        this->setIsScheduledForRemove(true);
    }
}
void Missile::update()
{
    
}