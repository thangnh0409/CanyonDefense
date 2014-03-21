#include "SelectSprite.h"
#include "SelectDifficuldScene.h"
#include "HelloWorldScene.h"
#include "LevelLayer.h"
//#include "Variable.h"
extern float scaleX;
extern float scaleY;
extern int selected;
extern int map;
extern int difficuld;
SelectSprite::SelectSprite(Layer* _parent,Texture2D* aTexture,ParallaxNode* _voidNode,Layer* _newLayer, int _indicator)
{
	indicator = _indicator;
	newLayer = _newLayer;
	voidNode = _voidNode;
	initWithTexture(aTexture);
	selected = 1;
	parentLayer = _parent;

	auto size = Director::getInstance()->getVisibleSize();
	auto closeItem1 = MenuItemImage::create(
                                           "back2.png",
                                           "next2.png",
                                           CC_CALLBACK_1(SelectSprite::backSelect, this));
    
	closeItem1->setPosition(Point(size.width/2 - scaleX*this->getContentSize().width/5 ,size.height/2+30*scaleY));
	menu1 = Menu::create(closeItem1, NULL);
    menu1->setPosition(Point(0,0));
	parentLayer->addChild(menu1, 1,10);
	menu1->setLocalZOrder(0);
	auto closeItem2 = MenuItemImage::create(
                                           "next2.png",
                                           "back2.png",
										   CC_CALLBACK_1(SelectSprite::nextSelect, this));
    
	closeItem2->setPosition(Point(size.width/2 + scaleX*this->getContentSize().width/5 ,size.height/2+30*scaleY));
    menu2 = Menu::create(closeItem2, NULL);
    menu2->setPosition(Point(0,0));
	parentLayer->addChild(menu2, 1,11);
}
void SelectSprite::nextSelect(Object* sender)
{
	if(selected == 1)
	{
		//parentLayer->addChild(menu1, 1,10);
		menu1->setLocalZOrder(1);
		voidNode->setPosition(-this->getContentSize().width/12*scaleX, voidNode->getPositionY());
		selected = 2;
		
	}else
	{
		if(selected == 2)
		{
			selected = 3;
			voidNode->setPosition(-this->getContentSize().width/6*scaleX, voidNode->getPositionY());
			//parentLayer->removeChildByTag(11,true);
			menu2->setLocalZOrder(0);
		}
	}
	
}
void SelectSprite::backSelect(Object* sender)
{
	if(selected == 3)
	{
		
		voidNode->setPosition(-this->getContentSize().width/12*scaleX, voidNode->getPositionY());
		selected = 2;
		menu2->setLocalZOrder(1);
		//parentLayer->addChild(menu2, 1,11);
	}else
	{
		if(selected == 2)
		{
			selected = 1;
			voidNode->setPosition(0,voidNode->getPositionY());
			menu1->setLocalZOrder(0);
			//parentLayer->removeChildByTag(10,true);
		}
	}
}
SelectSprite::~SelectSprite(void)
{
}

Rect SelectSprite::getRect()
{
    auto s = getTexture()->getContentSize();
    return Rect(-s.width / 2, -s.height / 2, s.width, s.height);
}


bool SelectSprite::initWithTexture(Texture2D* aTexture)
{
    if( Sprite::initWithTexture(aTexture) )
    {
        _state = kPaddleStateUngrabbed;
    }
    
    return true;
}

void SelectSprite::onEnter()
{
    Sprite::onEnter();
    
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
	
    listener->onTouchBegan = CC_CALLBACK_2(SelectSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SelectSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SelectSprite::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SelectSprite::onExit()
{
//    auto director = Director::getInstance();
//    director->getTouchDispatcher()->removeDelegate(this);
    Sprite::onExit();
}    

bool SelectSprite::containsTouchLocation(Touch* touch)
{
    return getRect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool SelectSprite::onTouchBegan(Touch* touch, Event* event)
{
    //CCLOG("Paddle::onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
    if (_state != kPaddleStateUngrabbed) return false;
    if ( !containsTouchLocation(touch) ) return false;
    
    _state = kPaddleStateGrabbed;
    
    return true;
}

void SelectSprite::onTouchMoved(Touch* touch, Event* event)
{
    
  //  CCLOG("Paddle::onTouchMoved id = %d, x = %f, y = %f", touch->getID(), touch->getLocation().x, touch->getLocation().y);
    
   // CCASSERT(_state == kPaddleStateGrabbed, "Paddle - Unexpected state!");    
	_state = kPaddleStateMove;
    //auto touchPoint = touch->getLocation();
    
    //setPosition( Point(touchPoint.x, getPosition().y) );
    auto diff = touch->getDelta();
    

	//auto node = getVoidNode();
	auto currentPos = voidNode->getPosition();
	if( currentPos.x > 0){
		voidNode->setPosition(0, currentPos.y);
	}else{ 
		if( currentPos.x < (-this->getContentSize().width/6)*scaleX){
			voidNode->setPosition(-this->getContentSize().width/6*scaleX, currentPos.y);
		}else{
			voidNode->setPosition(currentPos.x + diff.x, currentPos.y);
		}
	}
}

void SelectSprite::onTouchEnded(Touch* touch, Event* event)
{
	auto size = Director::getInstance()->getVisibleSize();

		//auto node = getChildByTag(1);
	auto currentPos = voidNode->getPosition();
	if(_state == kPaddleStateMove)
	{
		
		// CCASSERT(_state == kPaddleStateMove, "Paddle - Unexpected state!"); 
		
		if( currentPos.x > (-this->getContentSize().width/24)*scaleX){
			selected = 1;
			voidNode->setPosition(0, currentPos.y);
			menu1->setLocalZOrder(0);
			menu2->setLocalZOrder(1);
		}else if( currentPos.x < (-this->getContentSize().width/8)*scaleX){
			selected = 3;
			voidNode->setPosition(-this->getContentSize().width/6*scaleX, currentPos.y);
			menu1->setLocalZOrder(1);
			menu2->setLocalZOrder(0);
		}else{
			selected = 2;
			voidNode->setPosition(-this->getContentSize().width/12*scaleX, currentPos.y);
			menu1->setLocalZOrder(1);
			menu2->setLocalZOrder(1);
		}
		_state = kPaddleStateUngrabbed;
	}else
	{
		
		_state = kPaddleStateUngrabbed;
		if(touch->getLocationInView().x < size.width/2 + this->getContentSize().width/6 && touch->getLocationInView().x > size.width/2-this->getContentSize().width/6){
	
		arriveLayer(newLayer);
		}
	}
      
    
} 

void SelectSprite::arriveLayer(Layer* _layer)
{
	switch(indicator)
	{
        case 1:{
            map = selected;
            _layer = new SelectDifficuldScene();
            auto newScene = Scene::create();
            newScene->addChild(_layer, 0);
            Director::getInstance()->pushScene(newScene);
            break;
        }
        case 2:{
            difficuld = selected;
            auto mainScene = LevelLayer::scene();
            Director::getInstance()->pushScene(mainScene);
            break;
        }
	}
	
}
