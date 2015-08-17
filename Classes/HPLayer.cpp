#include "HPLayer.h"
USING_NS_CC;

bool HPLayer :: init(){
	if ( !Layer :: init()){
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height/20);
	bg->setPosition(0, visibleSize.height-bg->getContentSize().height);
	addChild(bg,1);

	Sprite* HPSprite = Sprite::create("images/HP.png");
	HPProgress = ProgressTimer::create(HPSprite);
	HPProgress->setType(ProgressTimer::Type::BAR);
	HPProgress->setBarChangeRate(Vec2(1,0));
	HPProgress->setMidpoint(Vec2(0, 0));
	HPProgress->setPercentage(100.0f);
	HPProgress->setPosition(HPProgress->getContentSize().width, bg->getPositionY()+bg->getContentSize().height/2);
	addChild(HPProgress,2,HPTAG);

	score = Label::createWithTTF("000000", "fonts/msyhbd.ttc",20);
	score->setColor(Color3B::YELLOW);
	score->setPosition(bg->getContentSize().width-2*score->getContentSize().width, bg->getPositionY()+bg->getContentSize().height/2);
	addChild(score,2,SCORETAG);

	return true;
}