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
    
    auto glView = EGLView::getInstance();
	Size frameSize = glView->getFrameSize();
    
    float scaleFactorX = frameSize.width / 800;
    float scaleFactorY = frameSize.height / 480;
    
    this->setLevelType(NORMAL);
    this->addWave();
    //add test background
    auto bgTest = Sprite::create("map1.png");
    bgTest->setPosition(visibleSize.width/2, visibleSize.height/2);
    //addChild(bgTest, 2);
    
    
    //add map background
    
    TMXTiledMap* map = MapManager::shareMap()->getTileMap();
    //TMXTiledMap* map = TMXTiledMap::create("TileMap.tmx");
    TMXLayer* metaLayer = map->getLayer("meta");
    metaLayer->setVisible(false);
    map->setAnchorPoint(Point(0.5, 0.5));
    map->setScale(scaleFactorX, scaleFactorY);
    map->setPosition(visibleSize.width/2, visibleSize.height/2);
    addChild(map, -1 , 1);
    
    Size CC_UNUSED s = map->getContentSize();
    CCLOG("Contentsize: %f, %f", s.width, s.height);
    
    //SmallCarEnemy* enemy = SmallCarEnemy::create();
    auto enemySprite = Sprite::create("dragon_water_2.png");
    enemySprite->setScale(0.15);
    auto objectGroup = map->getObjectGroup("objects");
    auto& rootObj = objectGroup->getObjects();
    
    Value objectsVal = Value(rootObj);
    CCLOG("%s", objectsVal.getDescription().c_str());
    
    auto arrayPoint = PointArray::create(20);
    
    for (auto& obj : rootObj)
    {
        ValueMap& dict = obj.asValueMap();
        float x = dict["x"].asFloat();
        float y = dict["y"].asFloat();
        log("obj: x = %f, y = %f", x, y);
        rootPoint = Point(0, 150);
        arrayPoint->addControlPoint(Point(x ,y));
        //enemy->setScale(0.3);
        //addChild(enemy, 1);
    }
    
    enemySprite->setPosition(arrayPoint->getControlPointAtIndex(0));
    auto action = CatmullRomTo::create(20, arrayPoint);
    enemySprite->runAction(action);
    addChild(enemySprite, 1);
    //GameMediator::shareInstance()->getTargets()->addObject(enemy);
    
    
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
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(LevelLayer::onTouchesBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
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
            Tower* tower = HutBasicTower::create();
            tower->setPosition(pos);
            this->addChild(tower, 2);
            GameMediator::shareInstance()->getTowers()->addObject(tower);
            break;
        }
        case 2:{
            Tower* tower = HutBasicTower::create();
            tower->setPosition(pos);
            this->addChild(tower, 2);
            GameMediator::shareInstance()->getTowers()->addObject(tower);
            break;
        }
            
           
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
        if(target){
            target->setPosition(rootPoint);
            gm->getTargets()->addObject(target);
            this->addChild(target, 1);
        }

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
                
            }
        }
    }
}
