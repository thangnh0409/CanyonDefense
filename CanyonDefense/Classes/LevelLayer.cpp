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

#define kMapTag 3
#define HUT_BASIC_MONEY             200
#define HUT_ADVANCE_MONEY           300
#define CATAPULT_BASIC_MONEY        400
#define SACRED_OAK_ADVANCE_MONEY    500

extern int map;
extern int selected;

Scene* LevelLayer::scene()
{
    auto scene = Scene::create();
    
    GameMediator *gm = GameMediator::shareInstance();
    auto layer = LevelLayer::create();
    gm->setGameLayer(layer);
    
    GameHUD* gh = GameHUD::shareInstance();
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
    
    float scaleFactorX = frameSize.width / 800;
    float scaleFactorY = frameSize.height / 480;
    //this->setScale(scaleFactorX, scaleFactorY);
    
    log("map id: %d, select: %d", map, selected);
    
    GameMediator::shareInstance()->setLevelMap(selected);
    GameMediator::shareInstance()->setMapID(map);
    this->addWave();
    
    //add map background
    
    TMXTiledMap* tileMap = MapManager::shareMap()->getTileMap();
    //TMXTiledMap* map = TMXTiledMap::create("TileMap.tmx");
    TMXLayer* metaLayer = tileMap->getLayer("meta");
    metaLayer->setVisible(false);
    tileMap->setAnchorPoint(Point(0.5, 0.5));
    tileMap->setScale(scaleFactorX, scaleFactorY);
    tileMap->setPosition(visibleSize.width/2, visibleSize.height/2);
    addChild(tileMap, -1 , 1);
    
    Size CC_UNUSED s = tileMap->getContentSize();
    CCLOG("Contentsize: %f, %f", s.width, s.height);
    
    Animation* animationTest = Animation::create();
    animationTest->addSpriteFrameWithFile("dragon_1_1.png");
    animationTest->addSpriteFrameWithFile("dragon_1_2.png");
    animationTest->addSpriteFrameWithFile("dragon_1_3.png");
    animationTest->addSpriteFrameWithFile("hut_2.png");
    animationTest->addSpriteFrameWithFile("dragon_1_5.png");
    animationTest->addSpriteFrameWithFile("dragon_1_6.png"  );
    
    animationTest->setDelayPerUnit(4 / 6);
    animationTest->setRestoreOriginalFrame(true);
    animationTest->setLoops(1);

    Sprite* spr = Sprite::create("dragon_1_1.png");
    spr->setScale(1.5);
    spr->setPosition(Point(150, 170));
    this->addChild(spr, 3);
    spr->runAction(RepeatForever::create(Animate::create(animationTest)));
    
    //load textTure
    
    Vector<SpriteFrame*> aniFrame(6);
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("yulei1.plist", "yulei1.png");
    
//    auto missile = Sprite::create("Player.png");
//    missile->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
//    this->addChild(missile, 1);
//    char str[100] = {0};
//    for (int i = 1; i < 6; i++) {
//        sprintf(str, "yulei1_0%d.png", i);
//        auto frame = cache->getSpriteFrameByName(str);
//        aniFrame.pushBack(frame);
//    }
//    Animation* animation = Animation::createWithSpriteFrames(aniFrame, 0.3f);
//    missile->runAction(RepeatForever::create(Animate::create(animation)));
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(LevelLayer::onTouchesBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    scheduleUpdate();
    
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
            
            
           
        default:
            break;
    }
}

void LevelLayer::addWave()
{
    GameMediator* gm = GameMediator::shareInstance();
    switch (gm->getLevelMap()) {
        case EASY:{
            Wave* wave1 = Wave::create(1, 4, 2, 2);
            gm->getWaves()->addObject(wave1);
            Wave* wave2 = Wave::create(1, 4, 5, 3);
            gm->getWaves()->addObject(wave2);
//            Wave* wave3 = Wave::create(1, 4, 2, 3);
//            gm->getWaves()->addObject(wave3);
//            Wave* wave4 = Wave::create(1, 2, 0, 4);
//            gm->getWaves()->addObject(wave4);
            break;
        }
        case NORMAL:{
            Wave* wave1 = Wave::create(1, 4, 2, 2);
            gm->getWaves()->addObject(wave1);
            Wave* wave2 = Wave::create(1, 4, 5, 3);
            gm->getWaves()->addObject(wave2);
            Wave* wave3 = Wave::create(1, 4, 2, 3);
            gm->getWaves()->addObject(wave3);
            Wave* wave4 = Wave::create(1, 2, 0, 4);
            gm->getWaves()->addObject(wave4);
            break;
        }
        case HARD:{
            Wave* wave1 = Wave::create(1, 4, 2, 2);
            gm->getWaves()->addObject(wave1);
            Wave* wave2 = Wave::create(1, 4, 5, 3);
            gm->getWaves()->addObject(wave2);
            Wave* wave3 = Wave::create(1, 4, 2, 3);
            gm->getWaves()->addObject(wave3);
            Wave* wave4 = Wave::create(1, 2, 0, 4);
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
    log("waves count: %zd, current waves: %d", GameMediator::shareInstance()->getWaves()->count(), _currentWaveCount);
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
        if (wave->getNumSmallCar() > 0) {
            target = SmallDragonEnemy::create();
            wave->setNumSmallCar(wave->getNumSmallCar() - 1);
        }else
        if (wave->getNumMediumCar() > 0) {
            target = MediumDragonEnemy::create();
            wave->setNumMediumCar(wave->getNumMediumCar() - 1);
        }else
        if (wave->getNumBigCar() > 0){
            target = SmallFlyDragonEnemy::create();
            wave->setNumBigCar(wave->getNumBigCar() - 1);
        }else
            unschedule(schedule_selector(LevelLayer::addEnemy));
        
        if(target){
            //target->setPosition(rootPoint);
            gm->getTargets()->addObject(target);
            this->addChild(target, 1);
        }
    }
}

void LevelLayer::playGameAfterPause()
{
    
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
        && wave->getNumSmallCar() <= 0
        && wave->getNumMediumCar() <= 0
        && wave->getNumBigCar() <= 0) {
        unschedule(schedule_selector(LevelLayer::checkFinishWave));
        GameHUD::shareInstance()->showTimerCount();
        _gameState = WAIT_NEXT_WAVE;
        schedule(schedule_selector(LevelLayer::levelLogic),
                 getTimerDelay(GameMediator::shareInstance()->getLevelMap()), 0, 0);
    }else
        log("NUM TARGETS = %zd", gm->getTargets()->count());
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
            return 5;
        case 2:
            return 60;
        case 3:
            return 40;
        default:
            return 40;
    }
}
void LevelLayer::update(float dt)
{
    
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
                
            }
        }
    }
}
