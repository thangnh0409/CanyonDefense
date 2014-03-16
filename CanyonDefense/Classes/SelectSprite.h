#ifndef __SELECT_SPRITE__
#define __SELECT_SPRITE__

#include "cocos2d.h"

USING_NS_CC;

typedef enum tagPaddleState 
{
    kPaddleStateGrabbed,
	kPaddleStateMove,
    kPaddleStateUngrabbed
} SelectSpriteState; 

class SelectSprite : public Sprite
{
protected:
	Layer* newLayer;
	Layer* parentLayer;
    SelectSpriteState        _state;
	 ParallaxNode* voidNode;
	 int indicator;
	 Menu* menu1;
	 Menu* menu2;
public:
	SelectSprite(Layer* _parent,Texture2D* aTexture,ParallaxNode* _voidNode,Layer* newLayer, int _indicator);
    virtual ~SelectSprite(void);

    Rect getRect();
    bool initWithTexture(Texture2D* aTexture);
    virtual void onEnter();
    virtual void onExit();
    bool containsTouchLocation(Touch* touch);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
	void arriveLayer(Layer* _layer);

	void nextSelect(Object* sender);
	void backSelect(Object* sender);
};


#endif //__SELECT_SPRITE__