#include "pausePopup.h"
USING_NS_CC;
#pragma execution_character_set("utf-8")

Scene* pausePopup::createScene(RenderTexture* bgTexture, char* menuImage){
	Scene* scene = Scene::create();
	
	return scene;
}

pausePopup* pausePopup::create(RenderTexture* bgTexture, char* menuImage){
	auto layer = new pausePopup();
	if (layer && layer->init(bgTexture, menuImage)){
		return layer;
	}
	layer = NULL;
	return NULL;
}

bool pausePopup::init(RenderTexture* bgTexture, char* menuImage){
	if (!Layer::init()){
		return false;
	}

	auto bgSprite = Sprite::createWithTexture(bgTexture->getSprite()->getTexture());
	bgSprite->setFlippedY(true);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bgSprite, 1);

	menuSprite = Sprite::create(menuImage);
	menuSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	menuSprite->setOpacity(200);
	this->addChild(menuSprite, 2);
	Action* popupAction = Sequence::create(ScaleTo::create(0.0f, 0.0f),
								ScaleTo::create(0.06f, 1.05f),
								ScaleTo::create(0.08f, 0.95f),
								ScaleTo::create(0.08f, 1.0f), NULL);
	menuSprite->runAction(popupAction);
	
	/*
	contentText = Label::createWithTTF("游戏暂停","fonts/msyhbd.ttc",30);
	contentText->setPosition(menuSprite->getContentSize().width/2, menuSprite->getContentSize().height/3*2);
	menuSprite->addChild(contentText);
	
	auto backLabel = Label::createWithTTF("返回游戏", "fonts/msyhbd.ttc", 20);
	backLabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_0(pausePopup::backToGame,this));
	backItem->setPosition(backItem->getContentSize().width*2,0);

	auto levelLabel = Label::createWithTTF("选择关卡", "fonts/msyhbd.ttc", 20);
	levelLabel->setColor(Color3B::BLACK);
	auto levelItem = MenuItemLabel::create(levelLabel, CC_CALLBACK_0(pausePopup::backToLevel,this));
	levelItem->setPosition(0, 0);

	auto overLabel = Label::createWithTTF("结束游戏", "fonts/msyhbd.ttc", 20);
	overLabel->setColor(Color3B::BLACK);
	auto overItem = MenuItemLabel::create(overLabel, CC_CALLBACK_0(pausePopup::endGame,this));
	overItem->setPosition(-overItem->getContentSize().width* 2, 0);

	auto menu = Menu::create(backItem, levelItem, overItem,NULL);
	menu->setPosition(menuSprite->getContentSize().width/2,backItem->getContentSize().height*2);
	menuSprite->addChild(menu,10);
	*/
	return true;
}

void pausePopup::backToGame(){
	//Action* popAction = Sequence::create(ScaleTo::create(0.08f, 1.0f), 
	//															ScaleTo::create(0.08f, 0.95f), 
	//															ScaleTo::create(0.06f, 1.05f),
	//															ScaleTo::create(0.0f, 0.0f), NULL);
	//this->runAction(popAction);
	//this->removeFromParentAndCleanup(true);
	Director::getInstance()->popScene();
}

void pausePopup::endGame(){
	Director::getInstance()->end();
}

void pausePopup::setContentText(String* str){
	contentText->setString(str->getCString());
}

void pausePopup::backToLevel(){
	auto scene = ChoseLevel::createScene();
	Director::getInstance()->replaceScene(CCTransitionCrossFade::create(1.0f, scene));
}