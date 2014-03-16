#ifndef __SELECTMAP_SCENE__
#define __SELECTMAP_SCENE__

#include "cocos2d.h"
#include "SelectSprite.h"

USING_NS_CC;

class SelectMapScene : public cocos2d::Layer 
{
protected:
	Sprite* cocosImage;
    ParallaxNode* voidNode;
	float _scaleX;
	float _scaleY;
	SelectSprite* paddle;
public:
	SelectMapScene(void);
	~SelectMapScene(void);

    void nextCallback(Object* sender);
    void backCallback(Object* sender);
	void update(float dt);
	//void nextSelect(Object* sender);
	//void backSelect(Object* sender);
};

#endif //__SELECTMAP_SCENE__