#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class MenuScene : public cocos2d::Layer
{
	
public:
    
    static Scene* createScene();
	virtual bool init();
    
    void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	void menuPlayCallback(Object* pSender);
	void menuHelpCallback(Object* pSender);
	void menuScoreCallback(Object* pSender);

	CREATE_FUNC(MenuScene);
};

#endif //__MENU_SCENE_H__