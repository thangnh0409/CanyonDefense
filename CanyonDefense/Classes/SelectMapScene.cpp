#include "SelectMapScene.h"
#include "SelectSprite.h"
//#include "Variable.h"
#include "MenuScene.h"
#include "SelectDifficuldScene.h"
extern float scaleX;
extern float scaleY;
extern int map;
extern int selected;
SelectMapScene::SelectMapScene(void)
{
	auto size = Director::getInstance()->getVisibleSize();

	auto backgroundSprite = Sprite::create("map_background.png");

	backgroundSprite->setScale(scaleX, scaleY);
	backgroundSprite->setPosition(size.width/2,size.height/2);
	
	this->addChild(backgroundSprite,1);

	auto backSpriteNormal = Sprite::create("back1.png");
    auto backSpriteSelected = Sprite::create("back1.png");
    auto backSpriteDisabled = Sprite::create("back1.png");
    
	auto item1 = MenuItemSprite::create(backSpriteNormal, backSpriteSelected, backSpriteDisabled, CC_CALLBACK_1(SelectMapScene::backCallback, this) );
    
	auto nextSpriteNormal = Sprite::create("next1.png");
    auto nextSpriteSelected = Sprite::create("next1.png");
    auto nextSpriteDisabled = Sprite::create("next1.png");

	auto item2 = MenuItemSprite::create(nextSpriteNormal, nextSpriteSelected, nextSpriteDisabled, CC_CALLBACK_1(SelectMapScene::nextCallback, this) );
    
	//Menu
	auto menu = Menu::create(item1,item2, NULL);
	menu->alignItemsHorizontallyWithPadding(-120);
	// elastic effect
    
    int i=0;
    for(const auto &child : menu->getChildren()) {
        auto dstPoint = child->getPosition();
        int offset = (int) (size.width/2 + 50);
        if( i % 2 == 0)
            offset = -offset;
        
        child->setPosition( Point( dstPoint.x + offset, dstPoint.y) );
        child->runAction(
                         EaseElasticOut::create(MoveBy::create(2, Point(dstPoint.x - offset,0)), 0.35f)
                         );
        i++;
    }
	
	menu->setPosition(Point(size.width/2, size.height - backgroundSprite->getContentSize().height/9*8*scaleY));
    this->addChild(menu,1);

	voidNode = ParallaxNode::create();
	auto paddleTexture = Director::getInstance()->getTextureCache()->addImage("map1.png");
    
	 paddle = new SelectSprite(this,paddleTexture, voidNode, new SelectDifficuldScene(), 1);
	 paddle->setScaleX(scaleX);
	 paddle->setScaleY(scaleY);
	 paddle->setPosition( Point(0,0));
//	auto size = Director::getInstance()->getWinSizeInPixels();
    // top image is moved at a ratio of 3.0x, 2.5y
	 voidNode->addChild( paddle, 2, Point(4.0f,1.5f), Point(size.width/2 + scaleX*paddle->getContentSize().width/3,size.height/2+30*scaleY) );
    addChild(voidNode, 0, 1);
	//this->addChild(paddle);
	
	
	//addChild(closeItem,2,10);
}
/*
void SelectMapScene::nextSelect(Object* sender)
{
	if(selected == 1)
	{
		
		voidNode->setPosition(-paddle->getContentSize().width/12*scaleX, voidNode->getPositionY());
		selected = 2;
	}else
	{
		if(selected == 2)
		{
			selected = 3;
			voidNode->setPosition(-paddle->getContentSize().width/6*scaleX, voidNode->getPositionY());
			this->removeChildByTag(10,true);
		}
	}
	
}
void SelectMapScene::backSelect(Object* sender)
{
	if(selected == 3)
	{
		
		voidNode->setPosition(-paddle->getContentSize().width/12*scaleX, voidNode->getPositionY());
		selected = 2;
	}else
	{
		if(selected == 2)
		{
			selected = 1;
			voidNode->setPosition(0,voidNode->getPositionY());
			this->removeChildByTag(11,true);
		}
	}
}
*/
void SelectMapScene::backCallback(Object* sender)
{
	Director::getInstance()->popScene();
}

void SelectMapScene::nextCallback(Object* sender)
{
	auto newScene = Scene::create();
	auto layer = new SelectDifficuldScene();
	newScene->addChild(layer, 0);
	Director::getInstance()->pushScene(newScene);
}
void SelectMapScene::update(float dt)
{
	
}
SelectMapScene::~SelectMapScene(void)
{
	
}
