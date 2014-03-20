#include "SelectDifficuldScene.h"
#include "SelectMapScene.h"
#include "SelectSprite.h"
#include "HelloWorldScene.h"
#include "LevelLayer.h"

//#include "Variable.h"
extern float scaleX;
extern float scaleY;
extern int map;
extern int difficuld;
extern int selected;
SelectDifficuldScene::SelectDifficuldScene(void)
{
	// lay map
	map = selected;
	//
	auto size = Director::getInstance()->getVisibleSize();

	auto backgroundSprite = Sprite::create("difficuld_background.png");

	backgroundSprite->setScaleX(scaleX);
	backgroundSprite->setScaleY(scaleY);
	backgroundSprite->setPosition(size.width/2,size.height/2);
	
	this->addChild(backgroundSprite,1);

	auto backSpriteNormal = Sprite::create("back1.png");
    auto backSpriteSelected = Sprite::create("back1.png");
    auto backSpriteDisabled = Sprite::create("back1.png");
	auto item1 = MenuItemSprite::create(backSpriteNormal, backSpriteSelected, backSpriteDisabled, CC_CALLBACK_1(SelectDifficuldScene::backCallback, this) );
    
	auto nextSpriteNormal = Sprite::create("next1.png");
    auto nextSpriteSelected = Sprite::create("next1.png");
    auto nextSpriteDisabled = Sprite::create("next1.png");

	auto item2 = MenuItemSprite::create(nextSpriteNormal, nextSpriteSelected, nextSpriteDisabled, CC_CALLBACK_1(SelectDifficuldScene::nextCallback, this) );
    
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
	auto paddleTexture = Director::getInstance()->getTextureCache()->addImage("difficuld1.png");
    
	SelectSprite* paddle = new SelectSprite(this,paddleTexture, voidNode, new LevelLayer(), 2);
	 paddle->setScaleX(scaleX);
	 paddle->setScaleY(scaleY);
	 paddle->setPosition( Point(0,0));
    // top image is moved at a ratio of 3.0x, 2.5y
	 voidNode->addChild( paddle, 2, Point(4.0f,1.5f), Point(size.width/2 + scaleX*paddle->getContentSize().width/3,size.height/2+30*scaleY) );
    addChild(voidNode, 0, 1);
	//this->addChild(paddle);
	
}

void SelectDifficuldScene::backCallback(Object* sender)
{
	Director::getInstance()->popScene();
}

void SelectDifficuldScene::nextCallback(Object* sender)
{
	auto mainGame = LevelLayer::scene();
	Director::getInstance()->pushScene(mainGame);
}



SelectDifficuldScene::~SelectDifficuldScene(void)
{
	
}
