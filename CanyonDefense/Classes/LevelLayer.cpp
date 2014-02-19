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
    projectileArray = new Array();
    
    // add sprite to screen
    
    auto missile = Missile::create("Player.png");
    missile->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    this->addChild(missile);
    
    auto enemy = Enemy::create("Target.png");
    
    return true;
}
void LevelLayer::update(float delta)
{
    
}