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
    this->setLevelType(NORMAL);
    this->addWave();
    
    //add map background
    
    TMXTiledMap* map = MapManager::shareMap()->getTileMap();
    TMXLayer* metaLayer = map->getLayer("meta");
    metaLayer->setVisible(false);
    addChild(map, -1 , 1);
    Size CC_UNUSED s = map->getContentSize();
    CCLOG("Contentsize: %f, %f", s.width, s.height);
    
    SmallCarEnemy* enemy = SmallCarEnemy::create();
    
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
        setRootPoint(Point(x, y));
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
    
    Tower* tower2 = MissileTurretTower::create();
    tower2->setPosition(Point(250, 100));
    this->addChild(tower2, 2);
    GameMediator::shareInstance()->getTowers()->addObject(tower2);
    
    schedule(schedule_selector(LevelLayer::levelLogic), 60.0f, 1, 0);
    schedule(schedule_selector(LevelLayer::addEnemy), 2.5f);
    
    return true;
}

void LevelLayer::levelLogic(float dt)
{
    Wave* wave = this->getCurrentWave();
    if ( wave && wave->getNumSmallCar() <= 0 && wave->getNumMediumCar() <= 0 && wave->getNumBigCar() <= 0) {
        this->getNextWave();
        log("call get next wave");
    }
}
void LevelLayer::addTower(cocos2d::Point pos, int towerTag)
{
    switch (towerTag) {
        case 1:{
            Tower* tower = MissileTurretTower::create();
            tower->setPosition(pos);
            this->addChild(tower, 2);
            GameMediator::shareInstance()->getTowers()->addObject(tower);
            break;
        }
        case 2:
            
            break;
        default:
            break;
    }
}

void LevelLayer::addWave()
{
    GameMediator* gm = GameMediator::shareInstance();
    Wave* wave1 = Wave::create(1, 4, 2, 0);
    gm->getWaves()->addObject(wave1);
    Wave* wave2 = Wave::create(1, 4, 4, 2);
    gm->getWaves()->addObject(wave2);
    Wave* wave3 = Wave::create(1, 4, 5, 4);
    gm->getWaves()->addObject(wave3);
    Wave* wave4 = Wave::create(1, 2, 4, 5);
    gm->getWaves()->addObject(wave4);

}
Wave* LevelLayer::getCurrentWave()
{
    Wave* currentWave = (Wave*)GameMediator::shareInstance()->getWaves()->getObjectAtIndex(getCurrentWaveCount());
    return currentWave;
}
void LevelLayer::getNextWave()
{
    setCurrentWaveCount(_currentWaveCount + 1);
    
}
void LevelLayer::addEnemy(float dt)
{
    //log("call add enemy");
    GameMediator* gm = GameMediator::shareInstance();
    Wave* wave = this->getCurrentWave();
    Enemy* target = NULL;
    if (wave) {
        if (wave->getNumSmallCar() > 0) {
            target = SmallCarEnemy::create();
            wave->setNumSmallCar(wave->getNumSmallCar() - 1);
        }else
        if (wave->getNumMediumCar() > 0) {
            target = MediumCarEnemy::create();
            wave->setNumMediumCar(wave->getNumMediumCar() - 1);
        }else
        if (wave->getNumBigCar() > 0){
            target = BigCarEnemy::create();
            wave->setNumBigCar(wave->getNumBigCar() - 1);
        }
        target->setPosition(getRootPoint());
        gm->getTargets()->addObject(target);
        this->addChild(target, 1);
    }
}
