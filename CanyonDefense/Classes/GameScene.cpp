//
//  GameScene.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/18/14.
//
//

#include "GameScene.h"
#include "LevelLayer.h"


Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto gameLayer = GameScene::create();
    scene->addChild(gameLayer);
    return scene;
}

void GameScene::onEnter()
{
    Director::getInstance()->replaceScene(LevelLayer::scene());
}