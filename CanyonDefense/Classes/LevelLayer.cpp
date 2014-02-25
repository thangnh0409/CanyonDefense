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
#include "MissileTurret.h"
#include "Enemy.h"
#include "GameMediator.h"
#include "Tower.h"

#define kMapTag 3

Scene* LevelLayer::scene()
{
    auto scene = Scene::create();
    auto layer = LevelLayer::create();
    GameMediator *gm = GameMediator::shareInstance();
    gm->setGameLayer(layer);
    
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
    
    // add sprite to screen
    
    //add map background
    
    map = TMXTiledMap::create("TileMap.tmx");
    metaLayer = map->getLayer("meta");
    addChild(map, -1 , 1);
    Size CC_UNUSED s = map->getContentSize();
    CCLOG("Contentsize: %f, %f", s.width, s.height);
    
    Enemy* enemy = Enemy::create("tank07_07.png");
    
    auto objectGroup = map->getObjectGroup("objects");
    auto& rootObj = objectGroup->getObjects();
    
    Value objectsVal = Value(rootObj);
    CCLOG("%s", objectsVal.getDescription().c_str());
    
    for (auto& obj : rootObj)
    {
        ValueMap& dict = obj.asValueMap();
        float x = dict["x"].asFloat();
        float y = dict["y"].asFloat();
        enemy->setPosition(Point(x, y));
        //enemy->setScale(0.3);
        addChild(enemy, 1);
        GameMediator::shareInstance()->getTargets()->addObject(enemy);
    }
    
    //load textTure
    
    Vector<SpriteFrame*> aniFrame(6);
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("yulei1.plist", "yulei1.png");
    
    auto missile = Sprite::create("Player.png");
    missile->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    this->addChild(missile, 1);
    char str[100] = {0};
    for (int i = 1; i < 6; i++) {
        sprintf(str, "yulei1_0%d.png", i);
        auto frame = cache->getSpriteFrameByName(str);
        aniFrame.pushBack(frame);
    }
    Animation* animation = Animation::createWithSpriteFrames(aniFrame, 0.3f);
    missile->runAction(RepeatForever::create(Animate::create(animation)));
    
    //add sample MissileTurret
    
    auto missileTurret = MissileTurret::create("Player.png");
    missileTurret->setPosition(Point(100, 100));
    this->addChild(missileTurret);
    
    // add Tower
    
    Tower* tower = MissileTurretTower::create();
    tower->setPosition(Point(150, 100));
    this->addChild(tower, 2);
    GameMediator::shareInstance()->getTowers()->addObject(tower);
    
    return true;
}
void LevelLayer::update(float delta)
{
    for (Object* child1 : this->getChildren()){
        GameObject* gameObject1 = dynamic_cast<GameObject*>(child1);
        if (gameObject1 != NULL) {
            //call update object
            gameObject1->update();
            
           // log("detect object 1");
            
            auto gameObj1Rect = Rect(gameObject1->getPositionX() - gameObject1->getContentSize().width/2,
                                     gameObject1->getPositionY() - gameObject1->getContentSize().height/2,
                                     gameObject1->getContentSize().width, gameObject1->getContentSize().height);
            
            for ( Object* child2 : this->getChildren()){
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
            
            Enemy* enemy = dynamic_cast<Enemy*>(gameObject1);
            if (enemy) {
                Point pos = enemy->getPosition();
                //log("pos : x = %f, y = %f", pos.x, pos.y);
                int x = (pos.x) / map->getTileSize().width;
                int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y) / map->getTileSize().height;
                //log("x = %d, y = %d", y, x);
                enemy->getNextDirection(maptrix, y, x);
               
                // xac dinh thuoc co thuoc pham vi anh huong cua sung ko
                for (Object* child : this->getChildren()) {
                    MissileTurret* missileTurret = dynamic_cast<MissileTurret*>(child);
                    if (missileTurret) {
                        if(missileTurret->detectIntrusion(enemy->getPosition())){
                            Missile* missile = Missile::create("Projectile.png");
                            missile->setPosition(missileTurret->getPosition());
                            missile->runAction(MoveTo::create(0.5, enemy->getPosition()));
                            this->addChild(missile);
                            GameMediator::shareInstance()->getProjectiles()->addObject(missile);
                        }
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
/*
bool LevelLayer::collisionWithTile(cocos2d::Point pos)
{
    int x = pos.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y) / map->getTileSize().height;
    Point tileCoord = Point(x, y);
    int tileGID = metaLayer->getTileGIDAt(tileCoord);

    if (tileGID) {
        //const auto& value = map->getPropertiesForGID(tileGID).asValueMap();
        log("GID:%i, Properties:%s", tileGID, map->getPropertiesForGID(tileGID).asValueMap()["Collidable"].asString().c_str());
        std::string collidable = map->getPropertiesForGID(tileGID).asValueMap()["Collidable"].asString();
        if (collidable.compare("true") == 0) {
            log("collision true");
            return true;
        }
    }
    return false;
}
*/
void LevelLayer::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    for( auto& touch : touches)
    {
        auto location = touch->getLocation();
        Size visibleSize = Director::getInstance()->getVisibleSize();
        Missile* missile = Missile::create("Projectile.png");
        missile->setPosition(Point(visibleSize.width/2, 0));
        this->addChild(missile);
        //projectileArray->addObject(missile);
        
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