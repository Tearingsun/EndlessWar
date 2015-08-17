#ifndef _STARTSCENE_H_
#define _STARTSCENE_H_
#include "cocos2d.h"
#include "ChoseLevel.h"
#include "pausePopup.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
USING_NS_CC_EXT;
USING_NS_CC;
#define database UserDefault::getInstance()
#pragma execution_character_set("utf-8")

class StartScene : public Layer{
public:
	Size visibleSize;
	virtual bool init();

	ControlSlider* slider;

	static Scene* createScene();

	void toChoseLevel();
	void toHelp();
	void toSetting();
	void preLoadMusic();
	void playBgMusic();
	void pauseBgMusic();
	void remuseBgMusic();
	void setBgMusicVol(float x);
	void resetGame();

	void musicSlider(Ref *sender, Control::EventType controlEvent);

	CREATE_FUNC(StartScene);
};
#endif