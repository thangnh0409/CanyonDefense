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
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto btnStart = MenuItemImage::create("btnShowMenu.png", "btnShowMenu1.png", CC_CALLBACK_1(GameHUD::onButtonShowMenuClick, this));
    btnStart->setPosition(Point::ZERO);
    auto startMenu = Menu::create(btnStart, NULL);
    startMenu->setPosition(Point(20, 20));
    startMenu->setAnchorPoint(Point::ZERO);
    this->addChild(startMenu);
    moveableSprite = Array::create();
    moveableSprite->retain();
    hudBackground = Sprite::create("hud.png");
    hudBackground->setAnchorPoint(Point::ZERO);
    hudBackground->setVisible(false);
    hudBackground->setPosition(Point(40, 5));
    this->addChild(hudBackground);
    Point hubPos = hudBackground->getPosition();
    
    //add MissileTurretTower
    auto sprite = Sprite::create("CannonTurret.png");
    sprite->setPosition(Point(hubPos.x + 50, hubPos.y + 10));
    sprite->setTag(1);
    hudBackground->addChild(sprite);
    moveableSprite->addObject(sprite);
    
    auto sprite1 = Sprite::create("CannonTurret.png");
    sprite1->setPosition(Point(hubPos.x + 100, hubPos.y + 10));
    sprite1->setTag(2);
    hudBackground->addChild(sprite1);
    moveableSprite->addObject(sprite1);
    //add tower sprite
    
    towerSprite = Sprite::create();
    this->addChild(towerSprite);
    
    //add image resources
    
    auto resource = Sprite::create("resource.png");
    resource->setPosition(Point(visibleSize.width/2 - resource->getContentSize().width, visibleSize.height - resource->getContentSize().height));
    this->addChild(resource);
    auto lives = Sprite::create("live.png");
    lives->setPosition(Point(visibleSize.width/2 - 15 + lives->getContentSize().width/2, resource->getPositionY()));
    this->addChild(lives);
    auto score = Sprite::create("scores.png");
    score->setPosition(Point(visibleSize.width/2 + lives->getContentSize().width + 10 + score->getContentSize().width/2, resource->getPositionY()));
    this->addChild(score);
    
    
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
            log("can't build in map");
            return false;
        }
    }
    return true;

}
void GameHUD::onButtonShowMenuClick(cocos2d::Object *sender)
{
    if(!hudBackground->isVisible()){
        hudBackground->setVisible(true);
    }else
        hudBackground->setVisible(false);
}
void GameHUD::onButtonBasicClick(cocos2d::Object *sender)
{
    MenuItemLabel* menuItem = (MenuItemLabel*)sender;
    hudChildBackground->setVisible(true);
    hudChildBackground->setPosition(menuItem->getPosition()+ Point(150, 20));
    Point hubPos = hudChildBackground->getPosition();
    if (moveableSprite->count() > 0) {
        moveableSprite->removeAllObjects();
    }
    hudChildBackground->removeAllChildrenWithCleanup(true);
    //add MissileTurretTower
    auto sprite = Sprite::create("CannonTurret.png");
    sprite->setPosition(Point(hubPos.x + 50, hubPos.y + 40));
    sprite->setTag(1);
    hudChildBackground->addChild(sprite);
    moveableSprite->addObject(sprite);
    
    auto sprite1 = Sprite::create("CannonTurret.png");
    sprite1->setPosition(Point(hubPos.x + 100, hubPos.y + 40));
    sprite1->setTag(2);
    hudChildBackground->addChild(sprite1);
    moveableSprite->addObject(sprite1);
}
void GameHUD::onButtonAdvanceClick(cocos2d::Object *sender)
{
    MenuItemLabel* menuItem = (MenuItemLabel*)sender;
    hudChildBackground->setVisible(true);
    hudChildBackground->setPosition(menuItem->getPosition()+ Point(150, 20));
}
void GameHUD::onButtonBuildingClick(cocos2d::Object *sender)
{
    MenuItemLabel* menuItem = (MenuItemLabel*)sender;
    log("menu pos x = %f, y=%f", menuItem->getPositionX(), menuItem->getPositionY());
    hudChildBackground->setVisible(true);
    hudChildBackground->setPosition(menuItem->getPosition() + Point(150, 20));

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