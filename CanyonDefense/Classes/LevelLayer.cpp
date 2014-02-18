//
//  LevelLayer.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "LevelLayer.h"

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
    return true;
}
void LevelLayer::update(float delta)
{
    
}