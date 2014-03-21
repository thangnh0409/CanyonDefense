//
//  GameHUD.h
//  CanyonDefense
//
//  Created by thang nguyen on 2/26/14.
//
//

#ifndef __CanyonDefense__GameHUD__
#define __CanyonDefense__GameHUD__

#include "cocos2d.h"

USING_NS_CC;

class GameHUD : public Layer {
private:
    Sprite* hudBackground;
    Sprite* hudChildBackground;
    Sprite* pauseBg;
    Sprite* levelCompleteBg;
    Sprite* levelFailBg;
    Sprite* towerSprite;
    bool towerIsSelected;
    Sprite* rangeSprite;
    Array* moveableSprite;
    LabelTTF* resourceLabel;
    LabelTTF* liveLabel;
    LabelTTF* waveLabel;
    LabelTTF* timeLabel;
    Menu* btnPlay;
    MenuItemImage* btnSoundItem;
    MenuItemImage* btnPauseItem;
    
    bool catapultUnlocked;

public:
    static GameHUD* shareInstance();
    bool init();
    void onButtonShowMenuClick(Object* sender);
    void onButtonBasicClick(Object* sender);
    void onButtonAdvanceClick(Object* sender);
    void onButtonBuildingClick(Object* sender);
    void onButtonPauseClick(Object* senser);
    void onButtonPlayClick(Object* sender);
    void onButtonResumeClick(Object* sender);
    void onButtonExitClick(Object* sender);
    void onButtonSoundClick(Object* sender);
    bool canBuilderInMap(Point pos);
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);
    
    void initForNewGame();
    void updateResource(int value);
    void updateLive();
    void updateWave();
    void updateTime(float dt);
    void showTimerCount();
    void hideTimerCount();
    
    CC_SYNTHESIZE(int, resources, Resources);
    CC_SYNTHESIZE(int, lives, Lives);
    CC_SYNTHESIZE(int, waves, Waves);
    CC_SYNTHESIZE(int, times, Time);
    CC_SYNTHESIZE(int, _numDragonIsKilled, NumDrgIsKilled);
CC_SYNTHESIZE(bool, _thorSkillAvailible, ThorSkillAvailble);

};

#endif /* defined(__CanyonDefense__GameHUD__) */
