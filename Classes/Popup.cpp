#include "Popup.h"
USING_NS_CC;
#pragma execution_character_set("utf-8")

Scene* Popup::createScene(RenderTexture* bgTexture,char* menuImage){
	Scene* scene = Scene::create();
	auto layer = Popup::create(bgTexture,menuImage);
	scene->addChild(layer,1);
	return scene;
}


Popup* Popup::create(RenderTexture* bgTexture, char* menuImage){
	auto layer = new Popup();
	if (layer && layer->init(bgTexture, menuImage)){
		return layer;
	}
	layer = NULL;
	return NULL;
}

bool Popup::init(RenderTexture* bgTexture, char* menuImage){
	if (!Layer::init()){
		return false;
	}

	auto bgSprite = Sprite::createWithTexture(bgTexture->getSprite()->getTexture());
	bgSprite->setFlippedY(true);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	bgSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bgSprite, 1);

	auto menuSprite = Sprite::create(menuImage);
	menuSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	menuSprite->setOpacity(200);
	this->addChild(menuSprite, 2);

	return true;
}

void Popup::backToGame(){
	//Action* popAction = Sequence::create(ScaleTo::create(0.08f, 1.0f), 
	//															ScaleTo::create(0.08f, 0.95f), 
	//															ScaleTo::create(0.06f, 1.05f),
	//															ScaleTo::create(0.0f, 0.0f), NULL);
	//this->runAction(popAction);
	//this->removeFromParentAndCleanup(true);
	Director::getInstance()->popScene();
}

void Popup::endGame(){
	Director::getInstance()->end();
}

void Popup::setContentText(String* str){
	contentText->setString(str->getCString());
}

void Popup::backToLevel(){
	auto scene = ChoseLevel::createScene();
	Director::getInstance()->replaceScene(CCTransitionCrossFade::create(1.0f, scene));
}