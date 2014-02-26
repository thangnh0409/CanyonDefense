//
//  GameMediator.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/24/14.
//
//

#ifndef __CanyonDefense__GameMediator__
#define __CanyonDefense__GameMediator__

#include "cocos2d.h"
#include "LevelLayer.h"
#include "MapManager.h"
#include "GameHUD.h"

USING_NS_CC;

class GameMediator : Object {
    
public:
    ~GameMediator();
    
    static GameMediator* shareInstance();
    bool init();
    
    CC_SYNTHESIZE_RETAIN(LevelLayer*, _gameLayer, GameLayer);
    CC_SYNTHESIZE_RETAIN(MapManager*, _mapGame, MapGame);
    CC_SYNTHESIZE_RETAIN(GameHUD*, _gameHUD, GameHUD);
    CC_SYNTHESIZE_RETAIN(Array*, _targets, Targets);
    CC_SYNTHESIZE_RETAIN(Array*, _towers, Towers);
    CC_SYNTHESIZE_RETAIN(Array*, _projectiles, Projectiles);
};

#endif /* defined(__CanyonDefense__GameMediator__) */
