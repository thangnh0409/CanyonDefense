//
//  GameHUD.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/26/14.
//
//

#include "GameHUD.h"
#include "GameMediator.h"
#include "MapManager.h"

static GameHUD* _shareContext;

GameHUD* GameHUD::shareInstance()
{
    if (!_shareContext) {
        _shareContext = new GameHUD;
        _shareContext->init();
    }
    return _shareContext;
}

bool GameHUD::init()
{
    moveableSprite = Array::create();
    moveableSprite->retain();
    hudBackground = Sprite::create("hud.png");
    hudBackground->setAnchorPoint(Point::ZERO);
    this->addChild(hudBackground);
    Point hubPos = hudBackground->getPosition();
    
    //add MissileTurretTower
    auto sprite = Sprite::create("CannonTurret.png");
    sprite->setPosition(Point(hubPos.x + 50, hubPos.y + 40));
    sprite->setTag(1);
    hudBackground->addChild(sprite);
    moveableSprite->addObject(sprite);
    
    auto sprite1 = Sprite::create("CannonTurret.png");
    sprite1->setPosition(Point(hubPos.x + 100, hubPos.y + 40));
    sprite1->setTag(2);
    hudBackground->addChild(sprite1);
    moveableSprite->addObject(sprite1);
    
    //add tower sprite
    
    towerSprite = Sprite::create();
    this->addChild(towerSprite);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameHUD::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameHUD::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameHUD::onTouchesEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

bool GameHUD::canBuilderInMap(cocos2d::Point pos)
{
    TMXTiledMap* map = MapManager::shareMap()->getTileMap();
    TMXLayer* metaLayer = map->getLayer("meta");
    int x = pos.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y) / map->getTileSize().height;
    Point tileCoord = Point(x, y);
    int tileGID = metaLayer->getTileGIDAt(tileCoord);
    
    if (tileGID) {
        //const auto& value = map->getPropertiesForGID(tileGID).asValueMap();
        log("GID:%i, Properties:%s", tileGID, map->getPropertiesForGID(tileGID).asValueMap()["Collidable"].asString().c_str());
        std::string collidable = map->getPropertiesForGID(tileGID).asValueMap()["Collidable"].asString();
        if (collidable.compare("true") == 0) {
            return false;
        }
    }
    return true;

}
void GameHUD::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    Touch* touch = touches[0];
	Point location = touch->getLocation();
    Rect bgRect = hudBackground->getBoundingBox();
    Sprite* newSprite = NULL;
    if(bgRect.containsPoint(location)){
        Object* child = NULL;
        CCARRAY_FOREACH(moveableSprite, child){
            Sprite* sprite = (Sprite*)child;
            Rect sprRect = sprite->getBoundingBox();
            if (sprRect.containsPoint(location)) {
                newSprite = Sprite::createWithTexture(sprite->getTexture());
                newSprite->setTag(sprite->getTag());
                newSprite->setPosition(sprite->getPosition());
                towerSprite = newSprite;
                this->addChild(newSprite);
            }
        }
    }
}
void GameHUD::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    Touch* touch = touches[0];
	Point location = touch->getLocation();
    if (towerSprite) {
        towerSprite->setPosition(location);
    }

}
void GameHUD::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    Touch* touch = touches[0];
	Point location = touch->getLocation();
    log("touch end: x= %f , y= %f", location.x, location.y);
    if (towerSprite) {
        Rect hudBgRect = hudBackground->getBoundingBox();
        if (!hudBgRect.containsPoint(location) && canBuilderInMap(location)) {
            GameMediator::shareInstance()->getGameLayer()->addTower(location, towerSprite->getTag());
        }
        
        this->removeChild(towerSprite, true);
        towerSprite = NULL;
    }
}