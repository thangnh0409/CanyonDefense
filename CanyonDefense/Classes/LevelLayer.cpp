//
//  LevelLayer.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "LevelLayer.h"
#include "GameObject.h"
#include "Missile.h"
#include "Enemy.h"

Scene* LevelLayer::scene()
{
    auto scene = Scene::create();
    auto layer = LevelLayer::create();
    scene->addChild(layer);
    return scene;
}
bool LevelLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    this->setTouchEnabled(true);
    this->scheduleUpdate();
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    //init array
    
    enemyArray = new Array();
    enemyArray->init();
    projectileArray = new Array();
    projectileArray->init();
    
    
    // add sprite to screen
    
    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    Director *director = Director::getInstance();
    sprite->setScale(director->getContentScaleFactor());
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    auto missile = Missile::create("Player.png");
    missile->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    this->addChild(missile, 1);
    
    Enemy *enemy = Enemy::create("Target.png");
    enemy->setPosition(Point(visibleSize.width/2+ 100, visibleSize.height - 100));
    this->addChild(enemy);
    enemyArray->addObject(enemy);
    
    //load textTure
    
    
    Vector<SpriteFrame*> aniFrame(6);
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("yulei1.plist", "yulei1.png");
    
    Sprite* sprite1 = Sprite::createWithSpriteFrameName("yulei1_01.png");
    sprite1->setPosition(Point(200, 200));
    auto spriteBacth = SpriteBatchNode::create("yulei1.png");
    spriteBacth->addChild(sprite1);
    addChild(spriteBacth);
    
    char str[100] = {0};
    for (int i = 1; i < 6; i++) {
        sprintf(str, "yulei1_0%d.png", i);
        auto frame = cache->getSpriteFrameByName(str);
        aniFrame.pushBack(frame);
    }
    Animation* animation = Animation::createWithSpriteFrames(aniFrame, 0.3f);
    missile->runAction(RepeatForever::create(Animate::create(animation)));
    
    return true;
}
void LevelLayer::update(float delta)
{
    Object* child1 = NULL;
    Object* child2 = NULL;
    CCARRAY_FOREACH( enemyArray, child1){
        GameObject* gameObject1 = dynamic_cast<GameObject*>(child1);
        if (gameObject1 != NULL) {
            //call update object
            //gameObject->update();
            
           // log("detect object 1");
            
            auto gameObj1Rect = Rect(gameObject1->getPositionX() - gameObject1->getContentSize().width/2,
                                     gameObject1->getPositionY() - gameObject1->getContentSize().height/2,
                                     gameObject1->getContentSize().width, gameObject1->getContentSize().height);
            
            CCARRAY_FOREACH( projectileArray, child2){
                auto gameObject2 = dynamic_cast<GameObject*>(child2);
                if (gameObject2) {
                    auto gameObj2Rect = Rect(gameObject2->getPositionX() - gameObject2->getContentSize().width/2,
                                             gameObject2->getPositionY() - gameObject2->getContentSize().height/2,
                                             gameObject2->getContentSize().width, gameObject2->getContentSize().height);
                    
                    if (gameObj1Rect.intersectsRect(gameObj2Rect)) {
                        gameObject2->handleCollisionWith(gameObject1);
                        
                    }
                }
            }
        }
    }
    
    for (Object* obj: this->getChildren()) {
        GameObject* gameObj = dynamic_cast<GameObject*>(obj);
        if(gameObj){
            
            if (gameObj->getScheduledForRemove()) {
                this->removeChild(gameObj, true);
                
            }
        }
        
    }
}

void LevelLayer::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    for( auto& touch : touches)
    {
        auto location = touch->getLocation();
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Missile* missile = Missile::create("Projectile.png");
        missile->setPosition(Point(visibleSize.width/2, 0));
        this->addChild(missile);
        projectileArray->addObject(missile);
        
        // calculate target
        int offX = location.x - missile->getPositionX();
        int offY = location.y - missile->getPositionY();
        int realX = visibleSize.width + missile->getContentSize().width/2;
        //tanx
        float ratio = (float)offY / (float)offX;
        int realY = realX*ratio + missile->getPositionY();
        // distance calculate
        float distance = sqrtf(realX*realX + realY*realY);
        // velocity = 480 frs/ 1sec
        int velocity = 480/1;
        float duration = distance / velocity;
        
        missile->runAction(Sequence::create(MoveTo::create(duration, Point(realX, realY)), CallFuncN::create(this, callfuncN_selector(LevelLayer::spriteMoveFinish)), NULL));
        
    }
}
void LevelLayer::spriteMoveFinish(Node *sender)
{
    Sprite *sprite = (Sprite*)sender;
    this->removeChild(sprite, true);
    log("remove bullet");
}