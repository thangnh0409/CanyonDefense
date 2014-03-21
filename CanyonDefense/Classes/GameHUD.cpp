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

#define HUT_BASIC_MONEY             100
#define HUT_ADVANCE_MONEY           300
#define CATAPULT_BASIC_MONEY        200
#define SACRED_OAK_ADVANCE_MONEY    120
#define THOR_TEMPLATE_MONEY         250
#define MISSILE_BUILDING            0

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
    if (!Layer::init()) {
        return false;
    }
    towerSprite = NULL;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    btnPauseItem = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(GameHUD::onButtonPauseClick, this));
    auto btnPause = Menu::create(btnPauseItem, NULL);
    btnPause->setPosition(Point(visibleSize.width - btnPauseItem->getContentSize().width/2, visibleSize.height - btnPauseItem->getContentSize().height/2));
    this->addChild(btnPause);
    
    
    moveableSprite = Array::create();
    moveableSprite->retain();
    _numDragonIsKilled = 0;
    
    hudBackground = Sprite::create("hud.png");
    hudBackground->setAnchorPoint(Point::ZERO);
    this->addChild(hudBackground);
    Point hubPos = hudBackground->getPosition();
    
    //add HutBasicTower
    auto sprite = Sprite::create("hut_2_menu.png");
    sprite->setPosition(Point(hubPos.x + 35, hubPos.y+30));
    sprite->setTag(HUT_BASIC_MONEY);
    //sprite->setScale(0.8);
    this->addChild(sprite);
    moveableSprite->addObject(sprite);
    
    //add CatapultTower
    auto sprite1 = Sprite::create("lock_menu.png");
    sprite1->setPosition(sprite->getPosition() + Point(30 + sprite1->getContentSize().width/2, -5));
    sprite1->setTag(CATAPULT_BASIC_MONEY);
    this->addChild(sprite1);
    moveableSprite->addObject(sprite1);
    
    //add lock
    
    auto sprLock = Sprite::create("lock_menu.png");
    sprLock->setPosition(sprite1->getPosition() + Point(30 + sprLock->getContentSize().width/2, 0));
    this->addChild(sprLock);

    //add SacredOakTower
    auto sprite2 = Sprite::create("sacred_oak_1_menu.png");
    sprite2->setPosition(sprLock->getPosition() + Point(50 + sprite2->getContentSize().width, 0));
    sprite2->setTag(SACRED_OAK_ADVANCE_MONEY);
    this->addChild(sprite2);
    moveableSprite->addObject(sprite2);
    
    
    
    auto sprite3 = Sprite::create("hut_1_menu.png");
    sprite3->setPosition(sprite2->getPosition() + Point(30 + sprite3->getContentSize().width, 0));
    sprite3->setTag(SACRED_OAK_ADVANCE_MONEY);
    this->addChild(sprite3);
    moveableSprite->addObject(sprite3);
    
    //add lock2
    
    auto sprLock2 = Sprite::create("lock_menu.png");
    sprLock2->setPosition(sprite3->getPosition() + Point(30 + sprLock2->getContentSize().width/2, 0));
    this->addChild(sprLock2);
    
    // add ThorTemple
    
    auto sprite4 = Sprite::create("temple_of_thor_menu.png");
    sprite4->setPosition(sprLock2->getPosition() + Point(40 + sprite4->getContentSize().width, 0));
    sprite4->setTag(THOR_TEMPLATE_MONEY);
    this->addChild(sprite4);
    moveableSprite->addObject(sprite4);
    
    //add lock3
    
    auto sprLock3 = Sprite::create("lock_menu.png");
    sprLock3->setPosition(sprite4->getPosition() + Point(30 + sprLock3->getContentSize().width/2, 0));
    this->addChild(sprLock3);
    
    //add lock4
    
    auto sprLock4 = Sprite::create("lock_menu.png");
    sprLock4->setPosition(sprLock3->getPosition() + Point(30 + sprLock4->getContentSize().width/2, 0));
    this->addChild(sprLock4);
    
    // add Missile Building
    
    auto sprite5 = Sprite::create("tenlua_menu.png");
    sprite5->setPosition(sprLock4->getPosition() + Point(40 + sprite5->getContentSize().width, 0));
    sprite5->setTag(MISSILE_BUILDING);
    this->addChild(sprite5);
    moveableSprite->addObject(sprite5);
    
    //add lock5
    
    auto sprLock5 = Sprite::create("lock_menu.png");
    sprLock5->setPosition(sprite5->getPosition() + Point(30 + sprLock5->getContentSize().width/2, 0));
    this->addChild(sprLock5);
    
    //add lock6
    
    auto sprLock6 = Sprite::create("lock_menu.png");
    sprLock6->setPosition(sprLock5->getPosition() + Point(30 + sprLock6->getContentSize().width/2, 0));
    this->addChild(sprLock6);
    
    //add image resources
    
    auto resource = Sprite::create("resource.png");
    resource->setPosition(Point(resource->getContentSize().width, visibleSize.height - resource->getContentSize().height));
    this->addChild(resource);
    // add label
    
    resources = 500;
    resourceLabel = LabelTTF::create("300", "Marker Felt", 25);
    resourceLabel->setPosition(Point(resource->getPositionX() + resource->getContentSize().width + 30, resource->getPositionY()));
    resourceLabel->setColor(Color3B(0, 0, 255));
    this->addChild(resourceLabel, 1);
    
    auto livesSpr = Sprite::create("lives.png");
    livesSpr->setPosition(resourceLabel->getPosition() + Point(resourceLabel->getContentSize().width + livesSpr->getContentSize().width/2, 0));
    this->addChild(livesSpr);
    
    lives = 10;
    liveLabel = LabelTTF::create("10", "Marker Felt", 25);
    liveLabel->setPosition(Point(livesSpr->getPositionX() + livesSpr->getContentSize().width/2 + 20 , livesSpr->getPositionY()));
    liveLabel->setColor(Color3B(0, 0, 255));
    this->addChild(liveLabel);
    
    waves = 0;
    waveLabel = LabelTTF::create("Wave: 1", "Marker Felt", 25);
    waveLabel->setPosition(Point(liveLabel->getPositionX() + liveLabel->getContentSize().width + 40 , liveLabel->getPositionY()));
    waveLabel->setColor(Color3B(255, 0, 0));
    this->addChild(waveLabel);
    
    times = 60;
    timeLabel = LabelTTF::create("Time: 60", "Marker Felt", 25);
    timeLabel->setPosition(Point(visibleSize.width - timeLabel->getContentSize().width, timeLabel->getContentSize().height));
    timeLabel->setColor(Color3B(255, 0, 0));
    this->addChild(timeLabel);
    
    
    // add pause dialog
    
    pauseBg = Sprite::create("pause_bg.png");
    pauseBg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    pauseBg->setVisible(false);
    this->addChild(pauseBg, 100);
    auto resumeItem = MenuItemImage::create("resume.png", "resume_press.png", CC_CALLBACK_1(GameHUD::onButtonResumeClick, this));
    auto exitItem = MenuItemImage::create("exit.png", "exit_press.png", CC_CALLBACK_1(GameHUD::onButtonExitClick, this));
    
    auto menuDialog = Menu::create(resumeItem, exitItem, NULL);
    menuDialog->setPosition(Point(pauseBg->getContentSize().width/2, pauseBg->getContentSize().height/2));
    menuDialog->alignItemsVertically();
    pauseBg->addChild(menuDialog);
    
    // add level complete dialog
    
    levelCompleteBg = Sprite::create("level_complete.png");
    levelCompleteBg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    levelCompleteBg->setVisible(false);
    this->addChild(levelCompleteBg, 100);
    auto okItem = MenuItemImage::create("ok.png", "ok_press.png", CC_CALLBACK_1(GameHUD::onButtonExitClick, this));

    auto okMenu = Menu::create(okItem, NULL);
    okMenu->setPosition(Point(levelCompleteBg->getContentSize().width/2, levelCompleteBg->getContentSize().height/4));
    levelCompleteBg->addChild(okMenu);
    
    // add level fail dialog
    
    levelFailBg = Sprite::create("level_fail.png");
    levelFailBg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    levelFailBg->setVisible(false);
    auto okItem2 = MenuItemImage::create("ok.png", "ok_press.png", CC_CALLBACK_1(GameHUD::onButtonExitClick, this));
    auto okMenu2 = Menu::create(okItem2, NULL);
    okMenu2->setPosition(Point(levelFailBg->getContentSize().width/2, levelFailBg->getContentSize().height/4));
    this->addChild(levelFailBg, 100);
    levelFailBg->addChild(okMenu2);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(GameHUD::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameHUD::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameHUD::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(GameHUD::onTouchesCancelled, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void GameHUD::initForNewGame()
{
    resources = 500;
    lives = 10;
    waves = 0;
    times = 60;
    _numDragonIsKilled = 0;
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
    if (GameMediator::shareInstance()->getGameLayer()->getGameState() == RUNNING) {
        btnPauseItem->setNormalImage(Sprite::create("play.png"));
        GameMediator::shareInstance()->getGameLayer()->callPauseGame();
        pauseBg->setVisible(true);
    }else
        if (GameMediator::shareInstance()->getGameLayer()->getGameState() == WAIT_NEXT_WAVE) {
            btnPauseItem->setNormalImage(Sprite::create("pause2.png"));
            GameMediator::shareInstance()->getGameLayer()->setGameState(RUNNING);
            GameMediator::shareInstance()->getGameLayer()->playGameWhiteWaitNextWave();
        }
}
void GameHUD::onButtonPlayClick(cocos2d::Object *sender)
{
    
}
void GameHUD::onButtonResumeClick(cocos2d::Object *sender)
{
    pauseBg->setVisible(false);
    if (GameMediator::shareInstance()->getGameLayer()->getGameState() == PAUSE) {
        btnPauseItem->setNormalImage(Sprite::create("pause2.png"));
        GameMediator::shareInstance()->getGameLayer()->callResumeGame();
    }
}
void GameHUD::onButtonExitClick(cocos2d::Object *sender)
{
    GameMediator::shareInstance()->getGameLayer()->backScene();
    pauseBg->setVisible(false);
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
            switch (sprite->getTag()) {
                case HUT_BASIC_MONEY:
                    sprite->setTexture("hut_2_selected.png");
                    break;
                case HUT_ADVANCE_MONEY:
                    sprite->setTexture("hut_1_selected.png");
                    break;
                case MISSILE_BUILDING:
                {
                    if (!_thorSkillAvailible) {
                        return;
                    }
                    break;
                }
                default:
                    break;
            }
            towerIsSelected = true;
            newSprite = Sprite::createWithTexture(sprite->getTexture());
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
        if (towerSprite->getTag() == MISSILE_BUILDING && !hudBgRect.containsPoint(location) && towerIsSelected) {
            towerIsSelected = false;
            GameMediator::shareInstance()->getGameLayer()->addTower(location, towerSprite->getTag());
            if (rangeSprite) {
                this->removeChild(rangeSprite);
            }
            this->removeChild(towerSprite, true);
            
            towerSprite = NULL;
        }
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
                        sprite->setTexture("hut_2_menu.png");
                        break;
                    case HUT_ADVANCE_MONEY:
                        sprite->setTexture("hut_1_menu.png");
                        break;
                    default:
                        break;
                }
                
            }

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
     Mo khoa cho vu khi
     **/
    if (_numDragonIsKilled >= 10 && !catapultUnlocked) {
        Object* child = NULL;
        CCARRAY_FOREACH(moveableSprite, child){
            Sprite* sprite = (Sprite*)child;
            if (sprite->getTag() == CATAPULT_BASIC_MONEY) {
                sprite->setTexture("ballias_3.png");
                catapultUnlocked = true;
                break;
            }
        }
    }else
        if (_numDragonIsKilled >= 20) {
            
        }
    
    /**
     cap nhat xem co the dung duoc Tower hay ko
     */
    Object* child = NULL;
    CCARRAY_FOREACH(moveableSprite, child){
        Sprite* sprite = (Sprite*)child;
        int twMoney = sprite->getTag();
        //log("tower money : %d", twMoney);
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
    if (lives <= 0) {
        levelFailBg->setVisible(true);
        GameMediator::shareInstance()->getGameLayer()->setGameState(FINISH);
        GameMediator::shareInstance()->getGameLayer()->callPauseGame();
    }
    
}
void GameHUD::updateWave()
{
    waves ++;
    if (waves >= GameMediator::shareInstance()->getWaves()->count()) {
        levelCompleteBg->setVisible(true);
    }
    waveLabel->setString(String::createWithFormat("Wave: %d", waves)->getCString());
}
void GameHUD::updateTime(float dt)
{
    if (times > 0) {
        times--;
    }
    timeLabel->setString(String::createWithFormat("Time: %d", times)->getCString());
}
void GameHUD::showTimerCount()
{
    GameMediator* gm = GameMediator::shareInstance();
    btnPauseItem->setNormalImage(Sprite::create("play.png"));
    timeLabel->setVisible(true);
    times = gm->getGameLayer()->getTimerDelay(gm->getLevelMap());
    schedule(schedule_selector(GameHUD::updateTime), 1.0f);
}
void GameHUD::hideTimerCount()
{
    unschedule(schedule_selector(GameHUD::updateTime));
    timeLabel->setVisible(false);
    btnPauseItem->setNormalImage(Sprite::create("pause2.png"));
}