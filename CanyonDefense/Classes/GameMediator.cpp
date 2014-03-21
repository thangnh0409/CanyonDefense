//
//  GameMediator.cpp
//  CanyonDefense
//
//  Created by thang nguyen on 2/24/14.
//
//

#include "GameMediator.h"

static GameMediator _sharedContext;

GameMediator* GameMediator::shareInstance()
{
    static bool s_bFirstUse = true;
	if(s_bFirstUse){
		_sharedContext.init();
		s_bFirstUse = false;
	}
	return &_sharedContext;
}
GameMediator::~GameMediator(){
	CC_SAFE_RELEASE_NULL(_gameLayer);
    CC_SAFE_RELEASE_NULL(_gameHUD);
    CC_SAFE_RELEASE_NULL(_mapGame);
	CC_SAFE_RELEASE_NULL(_targets);
	
	CC_SAFE_RELEASE_NULL(_waves);
	CC_SAFE_RELEASE_NULL(_towers);
	CC_SAFE_RELEASE_NULL(_projectiles);
}


bool GameMediator::init()
{
    bool bRet = false;
    do {
        _gameLayer = NULL;
        _mapGame = NULL;
        _targets = Array::create();
        _targets->retain();
        _projectiles = Array::create();
        _projectiles->retain();
        _towers = Array::create();
        _towers->retain();
        _waves = Array::create();
        _waves->retain();
        
        bRet = true;
    } while (0);
    return bRet;
}