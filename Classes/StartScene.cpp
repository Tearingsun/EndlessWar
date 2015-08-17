#include "StartScene.h"
USING_NS_CC_EXT;
USING_NS_CC;

Scene* StartScene::createScene(){
	auto scene = Scene::create();
	auto layer = StartScene::create();
	scene->addChild(layer);
	return scene;
}

bool StartScene::init(){
	if (!Layer::init()){
		return false;
	}

	if (!database->getBoolForKey("isExit")){
		database->getBoolForKey("isExit",true);
		database->setFloatForKey("music",50.0f);
	}

	preLoadMusic();
	playBgMusic();

	visibleSize = Director::getInstance()->getVisibleSize();

	auto bgSprite = Sprite::create("images/bg.png");
	bgSprite->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(bgSprite,1);

	
	auto startItem = MenuItemImage::create("images/startButton.png","images/startButton.png",CC_CALLBACK_0(StartScene::toChoseLevel,this));
	auto startLabel = Label::createWithTTF("Start Game", "fonts/arial.ttf", 25);
	startItem->addChild(startLabel);
	startLabel->setPosition(startItem->getContentSize().width/3,startItem->getContentSize().height/2+startLabel->getContentSize().height/3);
	auto helpItem = MenuItemImage::create("images/startButton.png", "images/startButton.png", CC_CALLBACK_0(StartScene::toHelp,this));
	auto helpLabel= Label::createWithTTF("Introduce", "fonts/arial.ttf", 25);
	helpItem->addChild(helpLabel);
	helpLabel->setPosition(startItem->getContentSize().width / 3, startItem->getContentSize().height / 2 + startLabel->getContentSize().height / 3);
	auto settingItem = MenuItemImage::create("images/startButton.png", "images/startButton.png", CC_CALLBACK_0(StartScene::toSetting,this));
	auto settingLabel = Label::createWithTTF("Setting", "fonts/arial.ttf", 25);
	settingItem->addChild(settingLabel);
	settingLabel->setPosition(startItem->getContentSize().width / 3, startItem->getContentSize().height / 2 + startLabel->getContentSize().height / 3);

	startItem->setPosition(0,helpItem->getContentSize().height);
	settingItem->setPosition(0,-helpItem->getContentSize().height);
	helpItem->setPosition(0,0);
	auto startMenu = Menu::create(startItem,helpItem,settingItem,NULL);
	startMenu->setPosition(visibleSize.width/3, visibleSize.height/3);
	addChild(startMenu,2);
	

	return true;
}

void StartScene::toChoseLevel(){
	auto scene = ChoseLevel::createScene();
	Director::getInstance()->replaceScene(scene);
}

void StartScene::toHelp(){
	auto director = Director::getInstance();
	RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto scene = pausePopup::createScene(renderTexture, "images/popupBg1.png");
	auto layer = pausePopup::create(renderTexture, "images/popupBg1.png");
	scene->addChild(layer, 1);

	layer->contentText = Label::createWithTTF("游戏介绍", "fonts/msyhbd.ttc", 20);
	layer->contentText->setPosition(layer->menuSprite->getContentSize().width / 2, layer->menuSprite->getContentSize().height / 20 * 18);
	layer->menuSprite->addChild(layer->contentText);

	std::string str = "这是一款横版过关类游戏\n";
	str = str + "在游戏中你可以使用各种酷炫的技能消灭敌人，\n拾取强化道具，\n最后干掉boss，通过关卡！\n";
	str = str + "基本操作：\nW：跳\nD：前进（向右）\nA：后退（向左）\n点击右侧技能栏选择攻击技能\n";
	str = str + "点击屏幕进行攻击\n现在就开始你的战斗之旅吧！\n";

	auto infoLabel = Label::createWithTTF(str, "fonts/msyhbd.ttc", 20);
	infoLabel->setPosition(layer->menuSprite->getContentSize().width / 2, layer->menuSprite->getContentSize().height / 20 * 19-infoLabel->getContentSize().width/2);
	layer->menuSprite->addChild(infoLabel);

	auto backLabel = Label::createWithTTF("返回游戏", "fonts/msyhbd.ttc", 15);
	backLabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_0(pausePopup::backToGame, layer));
	backItem->setPosition(backItem->getContentSize().width * 3 / 2, 0);

	layer->menu = Menu::create(backItem, NULL);

	layer->menu->setPosition(layer->menuSprite->getContentSize().width / 2, layer->menuSprite->getContentSize().height / 12);
	layer->menuSprite->addChild(layer->menu, 10);


	//auto popupScene = Popup::createScene(renderTexture,"images/popupBg.png");
	//auto popup = Popup::create("images/popupBg.png");
	//this->addChild(popup, 10);
	//Action* popupAction = Sequence::create(ScaleTo::create(0.0f, 0.0f),
	//								ScaleTo::create(0.06f, 1.05f),
	//								ScaleTo::create(0.08f, 0.95f),
	//								ScaleTo::create(0.08f, 1.0f), NULL);
	//popup->runAction(popupAction);
	//Director::getInstance()->pause();
	director->pushScene(scene);
	//Director::getInstance()->replaceScene(popupScene);
}

void StartScene::toSetting(){
	auto director = Director::getInstance();
	RenderTexture* renderTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();

	auto scene = pausePopup::createScene(renderTexture, "images/popupBg1.png");
	auto layer = pausePopup::create(renderTexture, "images/popupBg1.png");
	scene->addChild(layer, 1);

	layer->contentText = Label::createWithTTF("游戏设置", "fonts/msyhbd.ttc", 20);
	layer->contentText->setPosition(layer->menuSprite->getContentSize().width / 2, layer->menuSprite->getContentSize().height / 5 * 4);
	layer->menuSprite->addChild(layer->contentText);

	auto musicSetting = Label::createWithTTF("音量设置", "fonts/msyhbd.ttc", 15);
	musicSetting->setPosition(layer->menuSprite->getContentSize().width / 3, layer->menuSprite->getContentSize().height / 5 * 3);
	layer->menuSprite->addChild(musicSetting);

	slider = ControlSlider::create("images/slider/slider1.png","images/slider/slider0.png","images/slider/slider2.png");
	slider->setPosition(layer->menuSprite->getContentSize().width / 3*2, layer->menuSprite->getContentSize().height / 5 * 3);
	slider->setMinimumValue(0.0f);
	slider->setMaximumValue(100.0f);

	auto initValue = 50.0f;
	if (database->getBoolForKey("isExit")) initValue = database->getFloatForKey("music");

	slider->setValue(initValue);
	slider->addTargetWithActionForControlEvents(this, cccontrol_selector(StartScene::musicSlider), Control::EventType::VALUE_CHANGED);
	layer->menuSprite->addChild(slider);

	auto resetLabel = Label::createWithTTF("重置游戏", "fonts/msyhbd.ttc", 15);
	MenuItemLabel* resetItem = MenuItemLabel::create(resetLabel, CC_CALLBACK_0(StartScene::resetGame,this));
	resetItem->setPosition(0, 0);
	auto resetMenu = Menu::create(resetItem,NULL);
	resetMenu->setPosition(layer->menuSprite->getContentSize().width / 3, layer->menuSprite->getContentSize().height / 5 * 2);
	layer->menuSprite->addChild(resetMenu);
	
	auto backLabel = Label::createWithTTF("返回游戏", "fonts/msyhbd.ttc", 15);
	backLabel->setColor(Color3B::BLACK);
	auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_0(pausePopup::backToGame, layer));
	backItem->setPosition(backItem->getContentSize().width * 3 / 2, 0);

	layer->menu = Menu::create(backItem, NULL);

	layer->menu->setPosition(layer->menuSprite->getContentSize().width / 2, layer->menuSprite->getContentSize().height / 12);
	layer->menuSprite->addChild(layer->menu, 10);


	//auto popupScene = Popup::createScene(renderTexture,"images/popupBg.png");
	//auto popup = Popup::create("images/popupBg.png");
	//this->addChild(popup, 10);
	//Action* popupAction = Sequence::create(ScaleTo::create(0.0f, 0.0f),
	//								ScaleTo::create(0.06f, 1.05f),
	//								ScaleTo::create(0.08f, 0.95f),
	//								ScaleTo::create(0.08f, 1.0f), NULL);
	//popup->runAction(popupAction);
	//Director::getInstance()->pause();
	director->pushScene(scene);
	//Director::getInstance()->replaceScene(popupScene);
}
void StartScene::musicSlider(Ref *sender, Control::EventType controlEvent){
	auto slider = (ControlSlider*)sender;
	auto currentValue = slider->getValue();
	setBgMusicVol(currentValue);
	if (!database->getBoolForKey("isExit")){
		database->setBoolForKey("isExit", true);
	}
	database->setFloatForKey("music", currentValue);
}

void StartScene::preLoadMusic(){
	auto bgMusicPath = String::createWithFormat("%s", "sounds/bgMusic_ 0.mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bgMusicPath->getCString());
}

void StartScene::setBgMusicVol(float x){
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(x/100);
	if (x <= 0.01f) pauseBgMusic();
	else remuseBgMusic();
}

void StartScene::playBgMusic(){
	auto bgMusicPath = String::createWithFormat("%s", "sounds/bgMusic_0.mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic(bgMusicPath->getCString(), true);
	if (database->getBoolForKey("isExit")) setBgMusicVol(database->getFloatForKey("music"));
	else setBgMusicVol(50.0f);
}
void StartScene::pauseBgMusic(){
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void StartScene::remuseBgMusic(){
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void StartScene::resetGame(){
	if (!database->getBoolForKey("isExit")){
		return;
	}
	else{
		database->setBoolForKey("isExit", false);
		database->setIntegerForKey("nowLevel",0);
		database->setIntegerForKey("nowStage",0);
		database->setIntegerForKey("maxScore",0);
		database->setFloatForKey("music",50.0f);
		slider->setValue(50.0f);
	}
}



