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
    Sprite* towerSprite;
    Array* moveableSprite;
public:
    static GameHUD* shareInstance();
    bool init();
    void onButtonShowMenuClick(Object* sender);
    void onButtonBasicClick(Object* sender);
    void onButtonAdvanceClick(Object* sender);
    void onButtonBuildingClick(Object* sender);
    bool canBuilderInMap(Point pos);
    void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
};

#endif /* defined(__CanyonDefense__GameHUD__) */
