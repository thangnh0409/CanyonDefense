#ifndef __HELP_SCENE__
#define __HELP_SCENE__

#include "cocos2d.h"

USING_NS_CC;

class HelpScene : public cocos2d::Layer
{
public:
	HelpScene(void);
	~HelpScene(void);

	virtual bool init();
	
	void helpBackCallback(Object* pSender);
	void helpNextCallback(Object* pSender);

	CREATE_FUNC(HelpScene);
};

#endif //__HELP_SCENE__

