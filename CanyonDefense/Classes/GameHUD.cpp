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
#include "Tower.h"

#define HUT_BASIC_MONEY             200
#define HUT_ADVANCE_MONEY           300
#define CATAPULT_BASIC_MONEY        400
#define SACRED_OAK_ADVANCE_MONEY    500

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
    towerSprite = NULL;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto btnStart = MenuItemImage::create("btnShowMenu.png", "btnShowMenu1.png", CC_CALLBACK_1(GameHUD::onButtonShowMenuClick, this));
    btnStart->setPosition(Point::ZERO);
    auto startMenu = Menu::create(btnStart, NULL);
    startMenu->setPosition(Point(20, 20));
    startMenu->setAnchorPoint(Point::ZERO);
    this->addChild(startMenu);
    
    auto btnPauseItem = MenuItemImage::create("pause2.png", "pause1.png", CC_CALLBACK_1(GameHUD::onButtonPauseClick, this));
    auto btnPause = Menu::create(btnPauseItem, NULL);
    btnPause->setPosition(Point(visibleSize.width - btnPauseItem->getContentSize().width/2, visibleSize.height - btnPauseItem->getContentSize().height/2));
    this->addChild(btnPause);
    
//    auto playItem = MenuItemImage::create("play1.png", "play2.png", CC_CALLBACK_1(GameHUD::onButtonPlayClick, this));
//    btnPlay = Menu::create(playItem, NULL);
//    btnPlay->setPosition(Point(visibleSize.width - playItem->getContentSize().width/2 , playItem->getContentSize().height));
//    this->addChild(btnPlay);
    
    moveableSprite = Array::create();
    moveableSprite->retain();
    hudBackground = Sprite::create("hud.png");
    hudBackground->setAnchorPoint(Point::ZERO);
    hudBackground->setVisible(false);
    hudBackground->setPosition(Point(40, 5));
    this->addChild(hudBackground);
    Point hubPos = hudBackground->getPosition();
    
    //add HutBasicTower
    auto sprite = Sprite::create("hut_2.png");
    sprite->setPosition(Point(hubPos.x + 50, hubPos.y + 50));
    sprite->setTag(HUT_BASIC_MONEY);
    //sprite->setScale(0.8);
    this->addChild(sprite);
    moveableSprite->addObject(sprite);
    
    auto sprite1 = Sprite::create("hut_1.png");
    sprite1->setPosition(sprite->getPosition() + Point(100 + sprite1->getContentSize().width, 0));
    sprite1->setTag(HUT_ADVANCE_MONEY);
    this->addChild(sprite1);
    moveableSprite->addObject(sprite1);
    
    //add CatapultTower
    
    auto sprite2 = Sprite::create("ballias_3.png");
    sprite2->setPosition(sprite1->getPosition() + Point(100 + sprite2->getContentSize().width, 0));
    sprite2->setTag(CATAPULT_BASIC_MONEY);
    this->addChild(sprite2);
    moveableSprite->addObject(sprite2);
    
    //add SacredOakTower
    
    auto sprite3 = Sprite::create("sacred_oak_1.png");
    sprite3->setPosition(sprite2->getPosition() + Point(100 + sprite3->getContentSize().width, 0));
    sprite3->setTag(SACRED_OAK_ADVANCE_MONEY);
    this->addChild(sprite3);
    moveableSprite->addObject(sprite3);
    
    
    
    //add image resources
    
    auto resource = Sprite::create("resource.png");
    resource->setPosition(Point(resource->getContentSize().width, visibleSize.height - resource->getContentSize().height));
    this->addChild(resource);
    // add label
    
    resources = 22200;
    resourceLabel = LabelTTF::create("300", "Marker Felt", 25);
    resourceLabel->setPosition(Point(resource->getPositionX() + resource->getContentSize().width + 30, resource->getPositionY()));
    resourceLabel->setColor(Color3B(255, 80, 20));
    this->addChild(resourceLabel, 1);
    
    auto livesSpr = Sprite::create("lives.png");
    livesSpr->setPosition(resourceLabel->getPosition() + Point(resourceLabel->getContentSize().width + livesSpr->getContentSize().width/2, 0));
    this->addChild(livesSpr);
    
    lives = 10;
    liveLabel = LabelTTF::create("10", "Marker Felt", 25);
    liveLabel->setPosition(Point(livesSpr->getPositionX() + livesSpr->getContentSize().width/2 + 20 , livesSpr->getPositionY()));
    liveLabel->setColor(Color3B(0, 0, 255));
    this->addChild(liveLabel);
    
    waves = 10;
    waveLabel = LabelTTF::create("Wave: 1", "Marker Felt", 25);
    waveLabel->setPosition(Point(liveLabel->getPositionX() + liveLabel->getContentSize().width + 40 , liveLabel->getPositionY()));
    waveLabel->setColor(Color3B(255, 0, 0));
    this->addChild(waveLabel);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameHUD::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameHUD::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameHUD::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(GameHUD::onTouchesCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

bool GameHUD::canBuilderInMap(cocos2d::Point pos)
{
    TMXTiledMap* map = MapManager::shareMap()->getTileMap();
    TMXLayer* metaLayer = map->getLayer("meta");
    int x = pos.x / map->getTileSize().width;
    int y = ((map->getMapSize().height * map->getTileSize().height) - pos.y) / map->getTileSize().height;
    log("x = %d, y =%d", x ,y);
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
void GameHUD::onButtonPauseClick(cocos2d::Object *senser){
    
}
void GameHUD::onButtonPlayClick(cocos2d::Object *sender)
{
    
}
void GameHUD::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    log("Touch begin");
    Touch* touch = touches[0];
	Point location = touch->getLocation();
    Rect bgRect = hudBackground->getBoundingBox();
    Sprite* newSprite = NULL;
    Object* child = NULL;
    CCARRAY_FOREACH(moveableSprite, child){
        Sprite* sprite = (Sprite*)child;
        Rect sprRect = sprite->getBoundingBox();
        if (sprRect.containsPoint(location) && sprite->getTag() <= resources) {
            towerIsSelected = true;
            newSprite = Sprite::createWithTexture(sprite->getTexture());
            switch (sprite->getTag()) {
                case HUT_BASIC_MONEY:
                    sprite->setTexture("hut_2_selected.png");
                    break;
                case HUT_ADVANCE_MONEY:
                    sprite->setTexture("hut_1_selected.png");
                    break;
                default:
                    break;
            }
            newSprite->setTag(sprite->getTag());
            newSprite->setPosition(sprite->getPosition());
            rangeSprite = Sprite::create("range.png");
            this->addChild(rangeSprite);
            towerSprite = newSprite;
            this->addChild(newSprite);
            log("selected tower");
        }
    }

    if(bgRect.containsPoint(location)){
        
    }
}
void GameHUD::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    Touch* touch = touches[0];
	Point location = touch->getLocation();
    if (towerSprite != NULL) {
        towerSprite->setPosition(location);
        rangeSprite->setPosition(location);
    }

}
void GameHUD::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    Touch* touch = touches[0];
	Point location = touch->getLocation();
    log("touch end: x= %f , y= %f", location.x, location.y);
    
    TMXTiledMap* map = MapManager::shareMap()->getTileMap();
    TMXLayer* metaLayer = map->getLayer("meta");
    int x = location.x / (map->getTileSize().width) ;
    int y = ((map->getMapSize().height * map->getTileSize().height) - location.y) / map->getTileSize().height;
    log("col = %d, row =%d", x ,y);
    Point tileCoord = Point(x, y);
    int tileGID = metaLayer->getTileGIDAt(tileCoord);
    
    if (tileGID) {
        //const auto& value = map->getPropertiesForGID(tileGID).asValueMap();
        log("GID:%i, Properties:%s", tileGID, map->getPropertiesForGID(tileGID).asValueMap()["Collidable"].asString().c_str());
        std::string collidable = map->getPropertiesForGID(tileGID).asValueMap()["Collidable"].asString();
        if (collidable.compare("true") == 0) {
            log("can't build in map");
        }
    }
    
    
    if (towerSprite) {
        bool haveTower = false;
        Object* tws = NULL;
        CCARRAY_FOREACH(GameMediator::shareInstance()->getTowers(), tws){
            Tower* tower = (Tower*)tws;
            if (tower){
                Rect twRect = tower->getBoundingBox();
                if (twRect.containsPoint(location)) {
                    haveTower = true;
                    break;
                }
            }
        }
        Rect hudBgRect = hudBackground->getBoundingBox();
        /**
         @De xay duoc Tower yeu cau thoa man cac dieu kien:
         * Vi tri khong phai la song
         * Chua co Tower nao dc xay
         * Khong dat o vi tri goc
         * phai chon tower truoc do
         */
        if (!hudBgRect.containsPoint(location) && canBuilderInMap(location) && !haveTower && towerIsSelected) {
            towerIsSelected = false;
            GameMediator::shareInstance()->getGameLayer()->addTower(location, towerSprite->getTag());
            if (rangeSprite) {
                this->removeChild(rangeSprite);
            }
            this->removeChild(towerSprite, true);
            
            towerSprite = NULL;
            Object* child = NULL;
            CCARRAY_FOREACH(moveableSprite, child){
                Sprite* sprite = (Sprite*)child;
                switch (sprite->getTag()) {
                    case HUT_BASIC_MONEY:
                        sprite->setTexture("hut_2.png");
                        break;
                    case HUT_ADVANCE_MONEY:
                        sprite->setTexture("hut_1.png");
                        break;
                    default:
                        break;
                }

            }
            log("tesst");

        }
       
    }
}
void GameHUD::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    log("Touch cancelled");
}
void GameHUD::updateResource(int value)
{
    resources = resources + value;
    resourceLabel->setString(String::createWithFormat("%d", resources)->getCString());
    
    /**
     cap nhat xem co the dung duoc Tower hay ko
     */
    Object* child = NULL;
    CCARRAY_FOREACH(moveableSprite, child){
        Sprite* sprite = (Sprite*)child;
        int twMoney = sprite->getTag();
        log("tower money : %d", twMoney);
        if (resources >= twMoney) {
            sprite->setOpacity(255);
        }else
            sprite->setOpacity(150);
    }

}
void GameHUD::updateLive()
{
    lives --;
    liveLabel->setString(String::createWithFormat("%d", lives)->getCString());
}
void GameHUD::updateWave()
{
    waves ++;
    waveLabel->setString(String::createWithFormat("Wave: %d", waves)->getCString());
}