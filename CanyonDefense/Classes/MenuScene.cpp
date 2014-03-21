#include "MenuScene.h"
#include "HelpScene.h"
#include "SelectMapScene.h"
#include "SelectDifficuldScene.h"
//#include "Variable.h"
USING_NS_CC;
extern float scaleX;
extern float scaleY;


Scene* MenuScene::createScene()
{
    auto scene = Scene::create();
    auto menuLayer = MenuScene::create();
    scene->addChild(menuLayer);
    return scene;
}


bool MenuScene::init()
{
	//////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto listener = EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(MenuScene::onKeyReleased, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    Size s = Director::getInstance()->getVisibleSize();
    Point org = Director::getInstance()->getVisibleOrigin();

	// add "HelloWorld" splash screen"
    auto sprite = Sprite::create("menu.png");
	//scale
	scaleX = s.width/sprite->getContentSize().width;
	scaleY = s.height/sprite->getContentSize().height;

    //this->setScale(scaleX, scaleY);
	sprite->setScaleX(scaleX);
	sprite->setScaleY(scaleY);
    // position the sprite on the center of the screen
    sprite->setPosition(Point(s.width/2 , s.height/2 ));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

	//Font Item
	auto playSpriteNormal = Sprite::create("play1.png");
    auto playSpriteSelected = Sprite::create("play1_press.png");
    auto playSpriteDisabled = Sprite::create("play1.png");
    
	auto item1 = MenuItemSprite::create(playSpriteNormal, playSpriteSelected, playSpriteDisabled, CC_CALLBACK_1(MenuScene::menuPlayCallback, this) );
    
	
	auto helpSpriteNormal = Sprite::create("help1.png");
    auto helpSpriteSelected = Sprite::create("help1_press.png");
    auto helpSpriteDisabled = Sprite::create("help1.png");

	auto item2 = MenuItemSprite::create(helpSpriteNormal, helpSpriteSelected, helpSpriteDisabled, CC_CALLBACK_1(MenuScene::menuHelpCallback, this) );

	auto scoreSpriteNormal = Sprite::create("score1.png");
    auto scoreSpriteSelected = Sprite::create("score1_press.png");
    auto scoreSpriteDisabled = Sprite::create("score1.png");

	auto item3 = MenuItemSprite::create(scoreSpriteNormal, scoreSpriteSelected, scoreSpriteDisabled, CC_CALLBACK_1(MenuScene::menuScoreCallback, this) );
    
	//Menu
	auto menu = Menu::create(item1, item2, item3, NULL);
    menu->setPosition(Point(org.x + s.width/2, s.height/3));
	menu->alignItemsVerticallyWithPadding(-60);
	// elastic effect
   
    
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
    this->addChild(menu,1);
    
	
	
	return true;
}
void MenuScene::menuPlayCallback(Object* pSender)
{
	auto newScene = Scene::create();
	auto layer = new SelectMapScene();
	newScene->addChild(layer,0);
	Director::getInstance()->pushScene(newScene);
}
void MenuScene::menuHelpCallback(Object* pSender)
{
	auto newScene = Scene::create();
	auto layer = HelpScene::create();
	newScene->addChild(layer,0);
	Director::getInstance()->replaceScene(newScene);
}
void MenuScene::menuScoreCallback(Object* pSender)
{
	auto newScene = Scene::create();
	auto layer = new SelectDifficuldScene();
	newScene->addChild(layer,0);
	Director::getInstance()->replaceScene(newScene);
}

void MenuScene::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)
    {
        Director::getInstance()->end();
    }
    else if (keycode == EventKeyboard::KeyCode::KEY_MENU)
    {
        log("MENU clicked!");
    }
}