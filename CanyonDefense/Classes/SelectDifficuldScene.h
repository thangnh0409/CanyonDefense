#ifndef __SELECTDIFFICULD_SCENE__
#define __SELECTDIFFICULD_SCENE__
#include "cocos2d.h"
USING_NS_CC;

class SelectDifficuldScene : public cocos2d::Layer
{
protected:
	Sprite* cocosImage;
    ParallaxNode* voidNode;
	float _scaleX;
	float _scaleY;

public:
	SelectDifficuldScene(void);
	~SelectDifficuldScene(void);

    void nextCallback(Object* sender);
    void backCallback(Object* sender);

};

#endif //__SELECTDIFFICULD_SCENE__