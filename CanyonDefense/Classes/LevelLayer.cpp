//
//  LevelLayer.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "LevelLayer.h"
#include "MapManager.h"
#include "Missile.h"
#include "MissileTurret.h"
#include "Enemy.h"
#include "GameMediator.h"
#include "Tower.h"
#include "GameHUD.h"
#include "Projectile.h"
#include "MenuScene.h"

#define kMapTag 3
#define HUT_BASIC_MONEY             100
#define HUT_ADVANCE_MONEY           300
#define CATAPULT_BASIC_MONEY        200
#define SACRED_OAK_ADVANCE_MONEY    120
#define THOR_TEMPLATE_MONEY         250
#define MISSILE_BUILDING            0


extern int map;
extern int selected;

Scene* LevelLayer::scene()
{
    auto scene = Scene::create();
    
    GameMediator* gm = GameMediator::shareInstance();
    LevelLayer* layer = LevelLayer::create();
    gm->setGameLayer(layer);
    
    GameHUD* gh = GameHUD::shareInstance();
    gh->initForNewGame();
    gm->setGameHUD(gh);
    
    scene->addChild(layer);
    scene->addChild(gh, 2);
    return scene;
}
bool LevelLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto glView = EGLView::getInstance();
	Size frameSize = glView->getFrameSize();
    
    //this->setScale(scaleFactorX, scaleFactorY);
    
    log("map id: %d, select: %d", map, selected);
    
    GameMediator::shareInstance()->setLevelMap(selected);
    GameMediator::shareInstance()->setMapID(map);
    this->addWave();
    
    //add map background
    MapManager* mapmanager = MapManager::shareMap();
    mapmanager->initMapWithMapId(map);
    TMXTiledMap* tileMap = mapmanager->getTileMap();
    float scaleFactorX = frameSize.width / (tileMap->getMapSize().width * tileMap->getTileSize().width );
    float scaleFactorY = frameSize.height / (tileMap->getMapSize().height * tileMap->getTileSize().height);
    //TMXTiledMap* map = TMXTiledMap::create("TileMap.tmx");
    TMXLayer* metaLayer = tileMap->getLayer("meta");
    metaLayer->setVisible(false);
    tileMap->setAnchorPoint(Point(0.5, 0.5));
    tileMap->setScale(scaleFactorX, scaleFactorY);
    tileMap->setPosition(visibleSize.width/2, visibleSize.height/2);
    addChild(tileMap, -1 , 1);
    
    Size CC_UNUSED s = tileMap->getContentSize();
    CCLOG("Contentsize: %f, %f", s.width, s.height);
    
    //load textTure

    cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("yulei1.plist", "yulei1.png");
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(LevelLayer::onTouchesBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    
    //init all value constructor
    _currentWaveCount = -1;
    /**
     cap nhat resource de set opacity cua tower image
     */
    GameHUD::shareInstance()->updateResource(0);
    GameHUD::shareInstance()->showTimerCount();
    _gameState = WAIT_NEXT_WAVE;
    schedule(schedule_selector(LevelLayer::levelLogic),
             getTimerDelay(GameMediator::shareInstance()->getLevelMap()), 0, 0);
    
    
    
    return true;
}

void LevelLayer::levelLogic(float dt)
{
    log("call level logic");
    this->getNextWave();
    GameHUD::shareInstance()->hideTimerCount();
    if (!isScheduled(schedule_selector(LevelLayer::addEnemy))) {
         schedule(schedule_selector(LevelLayer::addEnemy), 2.5f);
    }
    if (!isScheduled(schedule_selector(LevelLayer::checkFinishWave))) {
        schedule(schedule_selector(LevelLayer::checkFinishWave), 2.0f);
    }

}
void LevelLayer::addTower(cocos2d::Point pos, int towerTag)
{
    switch (towerTag) {
        case HUT_BASIC_MONEY:{
            Tower* tower = HutBasicTower::create();
            tower->setPosition(pos);
            this->addChild(tower, 2);
            GameMediator::shareInstance()->getTowers()->addObject(tower);
            GameHUD::shareInstance()->updateResource(- HUT_BASIC_MONEY);
            break;
        }
        case HUT_ADVANCE_MONEY:{
            Tower* tower = HutAdvanceTower::create();
            tower->setPosition(pos);
            this->addChild(tower, 2);
            GameMediator::shareInstance()->getTowers()->addObject(tower);
            GameHUD::shareInstance()->updateResource(- HUT_ADVANCE_MONEY);
            break;
        }
        case CATAPULT_BASIC_MONEY:{
            Tower* tower = CatapultTower::create();
            tower->setPosition(pos);
            this->addChild(tower, 2);
            GameMediator::shareInstance()->getTowers()->addObject(tower);
            GameHUD::shareInstance()->updateResource(- CATAPULT_BASIC_MONEY);
            break;
        }
        case SACRED_OAK_ADVANCE_MONEY:{
            Tower* tower = SacredOakTower::create();
            tower->setPosition(pos);
            this->addChild(tower, 2);
            GameMediator::shareInstance()->getTowers()->addObject(tower);
            GameHUD::shareInstance()->updateResource(- SACRED_OAK_ADVANCE_MONEY);
            break;
        }
        case THOR_TEMPLATE_MONEY:{
            Tower* tower = ThorTempleTower::create();
            tower->setPosition(pos);
            this->addChild(tower, 2);
            GameMediator::shareInstance()->getTowers()->addObject(tower);
            GameHUD::shareInstance()->updateResource(- THOR_TEMPLATE_MONEY);
            break;
        }
            
        case MISSILE_BUILDING:{
            ThorTempleProjectile* thorProjectile = ThorTempleProjectile::create(pos);
            this->addChild(thorProjectile);
            thorProjectile->createExplosion();
            
            break;
        }
            
           
        default:
            break;
    }
}

void LevelLayer::addWave()
{
    GameMediator* gm = GameMediator::shareInstance();
    switch (gm->getLevelMap()) {
        case EASY:{
            Wave* wave1 = Wave::create(1, 8, 4, 0, 0, 0);
            gm->getWaves()->addObject(wave1);
            Wave* wave2 = Wave::create(1, 4, 5, 3, 3, 2);
            gm->getWaves()->addObject(wave2);
            Wave* wave3 = Wave::create(1, 4, 2, 3, 1, 4);
            gm->getWaves()->addObject(wave3);
            Wave* wave4 = Wave::create(1, 2, 0, 4, 4, 6);
            gm->getWaves()->addObject(wave4);
            break;
        }
        case NORMAL:{
            Wave* wave1 = Wave::create(1, 4, 2, 2, 1, 3);
            gm->getWaves()->addObject(wave1);
            Wave* wave2 = Wave::create(1, 4, 5, 3, 3, 2);
            gm->getWaves()->addObject(wave2);
            Wave* wave3 = Wave::create(1, 4, 2, 3, 1, 4);
            gm->getWaves()->addObject(wave3);
            Wave* wave4 = Wave::create(1, 2, 0, 4, 4, 6);
            gm->getWaves()->addObject(wave4);
            break;
        }
        case HARD:{
            Wave* wave1 = Wave::create(1, 4, 2, 2, 1, 3);
            gm->getWaves()->addObject(wave1);
            Wave* wave2 = Wave::create(1, 4, 5, 3, 3, 2);
            gm->getWaves()->addObject(wave2);
            Wave* wave3 = Wave::create(1, 4, 2, 3, 1, 4);
            gm->getWaves()->addObject(wave3);
            Wave* wave4 = Wave::create(1, 2, 0, 4, 4, 6);
            gm->getWaves()->addObject(wave4);
            break;
        }
        default:
            break;
    }

}
Wave* LevelLayer::getCurrentWave()
{
    if (GameMediator::shareInstance()->getWaves()->count() > getCurrentWaveCount()) {
        Wave* currentWave = (Wave*)GameMediator::shareInstance()->getWaves()->getObjectAtIndex(getCurrentWaveCount());
        return currentWave;
    }
    return NULL;
}
void LevelLayer::getNextWave()
{
    _currentWaveCount = _currentWaveCount + 1;
    GameHUD::shareInstance()->updateWave();
    //log("waves count: %zd, current waves: %d", GameMediator::shareInstance()->getWaves()->count(), _currentWaveCount);
    if (_currentWaveCount >= GameMediator::shareInstance()->getWaves()->count()) {
        log("finish game");
        unschedule(schedule_selector(LevelLayer::checkFinishWave));
        unschedule(schedule_selector(LevelLayer::addEnemy));
    }
}
void LevelLayer::addEnemy(float dt)
{
    log("call add enemy");
    GameMediator* gm = GameMediator::shareInstance();
    Wave* wave = this->getCurrentWave();
    Enemy* target = NULL;
    if (wave) {
        if (wave->getNumSmallDragon() > 0) {
            target = SmallDragonEnemy::create();
            wave->setNumSmallDragon(wave->getNumSmallDragon() - 1);
        }else
        if (wave->getNumMediumDragon() > 0) {
            target = MediumDragonEnemy::create();
            wave->setNumMediumDragon(wave->getNumMediumDragon() - 1);
        }else
        if (wave->getNumBigDragon() > 0){
            target = SmallFlyDragonEnemy::create();
            wave->setNumBigDragon(wave->getNumBigDragon() - 1);
        }else
        if (wave->getNumSmallFlyDragon() > 0) {
            target = SmallFlyDragonEnemy::create();
            wave->setNumSmallFlyDragon(wave->getNumSmallFlyDragon() - 1);
        }else
        if(wave->getNumMediumFlyDragon() > 0){
            target = MediumFlyDragonEnemy::create();
            wave->setNumMediumFlyDragon(wave->getNumMediumFlyDragon() - 1);
        }else
            unschedule(schedule_selector(LevelLayer::addEnemy));
        
        if(target){
            //target->setPosition(rootPoint);
            gm->getTargets()->addObject(target);
            this->addChild(target, 1000);
        }
    }
}

void LevelLayer::callPauseGame()
{
    _gameState = PAUSE;
//    unscheduleAllSelectors();
//    unscheduleUpdate();
    actionManager = Director::getInstance()->getActionManager()->pauseAllRunningActions();
    schedulerManager = Director::getInstance()->getScheduler()->pauseAllTargets();
}
void LevelLayer::callResumeGame()
{
    _gameState = RUNNING;
    for (auto sp: actionManager) {
        Director::getInstance()->getActionManager()->resumeTarget(sp);
    }
    for (auto schl : schedulerManager) {
        Director::getInstance()->getScheduler()->resumeTarget(schl);
    }
    
}

void LevelLayer::backScene()
{
    for (auto sp: actionManager) {
        Director::getInstance()->getActionManager()->resumeTarget(sp);
    }
//    auto mainMenu = MenuScene::createScene();
//    
//    Director::getInstance()->replaceScene(mainMenu);
    Director::getInstance()->popScene();
    log("pop to select difficult");
}

void LevelLayer::playGameWhiteWaitNextWave()
{
    if (isScheduled(schedule_selector(LevelLayer::levelLogic))) {
        unschedule(schedule_selector(LevelLayer::levelLogic));
    }
    this->levelLogic(1);
}

void LevelLayer::checkFinishWave(float dt)
{
    GameMediator* gm = GameMediator::shareInstance();
    Wave* wave = this->getCurrentWave();
    if (gm->getTargets()->count() == 0 && wave
        && wave->getNumSmallDragon() <= 0
        && wave->getNumMediumDragon() <= 0
        && wave->getNumBigDragon() <= 0
        && wave->getNumSmallFlyDragon() <=0
        && wave->getNumMediumFlyDragon() <= 0 ){
        unschedule(schedule_selector(LevelLayer::checkFinishWave));
        GameHUD::shareInstance()->showTimerCount();
        _gameState = WAIT_NEXT_WAVE;
        schedule(schedule_selector(LevelLayer::levelLogic),
                 getTimerDelay(GameMediator::shareInstance()->getLevelMap()), 0, 0);
    }
}
bool LevelLayer::isOutOfBound(cocos2d::Point pos)
{
    TMXTiledMap* map = MapManager::shareMap()->getTileMap();
    Rect mapRect = map->getBoundingBox();
    if (!mapRect.containsPoint(pos)) {
        return true;
    }
    return false;
}
float LevelLayer::getTimerDelay(int levelMap)
{
    switch (levelMap) {
        case 1:
            return 60;
        case 2:
            return 60;
        case 3:
            return 40;
        default:
            return 40;
    }
}

void LevelLayer::addExplosion(cocos2d::Point pos, int projectileType)
{

    Vector<SpriteFrame*> aniFrame(6);
    auto missile = Sprite::createWithSpriteFrameName("yulei1_11.png");
    missile->setScale(0.3f);
    missile->setPosition(pos);
    this->addChild(missile, 1);
    char str[100] = {0};
    switch (projectileType) {
        case 0:{
            for (int i = 1; i < 3; i++) {
                sprintf(str, "yulei1_0%d.png", i);
                auto frame = cache->getSpriteFrameByName(str);
                aniFrame.pushBack(frame);
            }
            break;
        }
        case 1:{
            for (int i = 11; i < 15; i++) {
                sprintf(str, "yulei1_%d.png", i);
                auto frame = cache->getSpriteFrameByName(str);
                aniFrame.pushBack(frame);
            }
            break;
        }
        // vu no do ten lua cua ThoTemple tao ra
        case 2:{
            missile->setScale(0.8f);
            for (int i = 1; i < 6; i++) {
                sprintf(str, "yulei1_0%d.png", i);
                auto frame = cache->getSpriteFrameByName(str);
                aniFrame.pushBack(frame);
            }
            break;
        }
            
        default:
            break;
    }
    Animation* animation = Animation::createWithSpriteFrames(aniFrame, 0.1f);
    Animate* animate = Animate::create(animation);
    missile->runAction(Sequence::create(animate, CallFuncN::create(CC_CALLBACK_1(LevelLayer::spriteMoveFinish, this)), NULL));

}
void LevelLayer::spriteMoveFinish(cocos2d::Node *sender)
{
    Sprite* sprt = (Sprite*)sender;
    if (sprt) {
        sprt->stopAllActions();
        sprt->removeFromParentAndCleanup(true);
    }
}
void LevelLayer::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *unused_event)
{
    GameMediator* gm = GameMediator::shareInstance();
    Touch* touch = touches[0];
	Point location = touch->getLocation();
    Object* child = NULL;
    CCARRAY_FOREACH(gm->getTowers(), child){
        Tower* tower = (Tower*)child;
        if (tower) {
            Rect twRect = tower->getRect();
            if (twRect.containsPoint(location)) {
                if(tower->rangeTower->isVisible()){
                    tower->rangeTower->setVisible(false);
                }else
                    tower->rangeTower->setVisible(true);
            }
        }
    }
}
