#ifndef _PAUSEPOPUP_H_
#define _PAUSEPOPUP_H_
#include "cocos2d.h"
#include "ChoseLevel.h"
USING_NS_CC;
#pragma execution_character_set("utf-8")

class pausePopup : public Layer{
public:

	Size visibleSize;

	Label* contentText;

	Sprite* menuSprite;

	Menu* menu;

	static Scene* createScene(RenderTexture* bgTexture, char* bgImage);
	virtual bool init(RenderTexture* bgTexture, char* menuImage);
	static pausePopup* create(RenderTexture* bgTexture, char* menuImage);

	void backToGame();
	void endGame();
	void backToLevel();
	//void restart();

	void setContentText(String* str);

};
#endif