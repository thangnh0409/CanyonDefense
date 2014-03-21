#include "HelpScene.h"
#include "MenuScene.h"
USING_NS_CC;
extern float scaleX;
extern float scaleY;
HelpScene::HelpScene(void)
{
}

HelpScene::~HelpScene(void)
{
}

bool HelpScene::init()
{
	if(!Layer::init())
	{
		return false;
	}
	
	auto size = Director::getInstance()->getVisibleSize();

	auto backgroundSprite = Sprite::create("help.png");
    backgroundSprite->setScale(scaleX, scaleY);
	backgroundSprite->setPosition(size.width/2,size.height/2);
	
	this->addChild(backgroundSprite,0);

	auto backSpriteNormal = Sprite::create("back1.png");
    auto backSpriteSelected = Sprite::create("back1_press.png");
    auto backSpriteDisabled = Sprite::create("back1_press.png");

	auto item1 = MenuItemSprite::create(backSpriteNormal, backSpriteSelected, backSpriteDisabled, CC_CALLBACK_1(HelpScene::helpBackCallback, this) );
    
	//Menu
	auto menu = Menu::create(item1, NULL);
	menu->alignItemsVertically();
	// elastic effect
    auto s = Director::getInstance()->getWinSize();
    
    int i=0;
    for(const auto &child : menu->getChildren()) {
        auto dstPoint = child->getPosition();
        int offset = (int) (s.width/2 + 50);
        if( i % 2 == 0)
            offset = -offset;
        
        child->setPosition( Point( dstPoint.x + offset, dstPoint.y) );
        child->runAction(
                         EaseElasticOut::create(MoveBy::create(2, Point(dstPoint.x - offset,0)), 0.35f)
                         );
        i++;
    }

	menu->setPosition(Point(s.width/2, s.height/5));
    this->addChild(menu,1);

	return true;
}

void HelpScene::helpBackCallback(Object* pSender)
{
	//Director::getInstance()->popToSceneStackLevel(1);
	auto newScene = Scene::create();
	auto layer = MenuScene::create();
	newScene->addChild(layer,0);
	Director::getInstance()->replaceScene(newScene);
}

void HelpScene::helpNextCallback(Object* pSender)
{

}