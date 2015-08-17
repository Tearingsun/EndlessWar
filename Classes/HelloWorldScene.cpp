#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include <string>
#include <Vector>
using namespace CocosDenshion;
USING_NS_CC;

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
	return scene;
}

HelloWorld* HelloWorld::create(PhysicsWorld* world,int level,int stage)
{
	HelloWorld* pRet = new HelloWorld();
	if (pRet && pRet->init(world,level,stage)){
		pRet->autorelease();
		return pRet;
	}
	delete pRet;
	pRet = NULL;
	return NULL;
}

bool HelloWorld::init(PhysicsWorld* world,int _level, int _stage)
{
    if ( !Layer::init() )
    {
        return false;
    } 

	initPar();

	level = _level;
	stage = _stage;
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(HelloWorld::setLevel), "selectedLevel", NULL);
	dispatcher = Director::getInstance()->getEventDispatcher();

	preLoadMusic();
	playBgMusic();

	m_world = world;
	//m_world->setAutoStep(false);
	m_world->setGravity(Vect(0,-grivity));
	this->setAnchorPoint(Point(0,0));
    winSize = Director::getInstance()->getWinSize();
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	auto str = String::createWithFormat("%d", stage);
	log(str->getCString());

	auto map1path = String::createWithFormat("%s%d%s%d%s","images/level",level,"/bg",stage,".png");
	map1 = Sprite::create(map1path->getCString());
	map1->setPosition(map1->getContentSize().width/2+origin.x, map1->getContentSize().height/2+origin.y);
	this->addChild(map1,1,MAP1_TAG);
	auto map2path = String::createWithFormat("%s%d%s%d%s", "images/level", level, "/bg", stage, ".png");
	map2 = Sprite::create(map2path->getCString());
	map2->setPosition(map1->getContentSize().width+map2->getContentSize().width/2+origin.x, map2->getContentSize().height/2+origin.y);
	this->addChild(map2,1,MAP2_TAG);

	Sprite* edge1 = Sprite::create();
	edge1->setContentSize(visibleSize);
	PhysicsBody* edgeBody1 = PhysicsBody::createEdgeBox(Size(visibleSize.width*3, visibleSize.height));
	edgeBody1->setDynamic(false);
	edgeBody1->setGravityEnable(false);
	edgeBody1->setMass(100000.0f);
	edgeBody1->getShape(0)->setRestitution(0.0f);
	edgeBody1->setContactTestBitmask(0xFF);
	edgeBody1->setCategoryBitmask(0x04);
	edgeBody1->setCollisionBitmask(0xFF);
	edgeBody1->getShape(0)->setTag(EDGE_TAG);
	edge1->setPhysicsBody(edgeBody1);
	edge1->setPosition(visibleSize.width/2,visibleSize.height/2);
	addChild(edge1,1,EDGEONE_TAG);

	Sprite* edge = Sprite::create();
	edge->setContentSize(visibleSize);
	PhysicsBody* edgeBody = PhysicsBody::createEdgeBox(Size(visibleSize.width*3,visibleSize.height/6));
	edgeBody->setDynamic(false);
	edgeBody->setGravityEnable(false);
	edgeBody->setMass(100000.0f);
	edgeBody->getShape(0)->setRestitution(0.0f);
	edgeBody->setContactTestBitmask(0xFF);
	edgeBody->setCategoryBitmask(0x04);
	edgeBody->setCollisionBitmask(0xFF);
	edgeBody->getShape(0)->setTag(EDGE_TAG);
	edge->setPhysicsBody(edgeBody);
	edge->setPosition(visibleSize.width / 2, visibleSize.height / 12);
	addChild(edge, 1, EDGE_TAG);


	Sprite* floor = Sprite::create();
	PhysicsBody* floorBody = PhysicsBody::createEdgeSegment(Point(0, visibleSize.height / 10), Point(visibleSize.width, visibleSize.height / 10));
	floorBody->setDynamic(false);
	floorBody->setGravityEnable(false);
	floorBody->getShape(0)->setRestitution(0.0f);
	floorBody->setMass(100000.0f);
	floorBody->setCategoryBitmask(0x0003);
	floorBody->setContactTestBitmask(0x0003);
	floorBody->setCollisionBitmask(0x0001);
	floor->setPosition(0, visibleSize.height / 10);
	floor->setPhysicsBody(floorBody);
	//this->addChild(floor,2,FLOOR_TAG);

	setEnemyFrameNumber();
	setPlayerFrameNumber();
	initDamage();

	player1 = new player();
	auto playerPath = String::createWithFormat("%s%d%s", "images/level", level, "/player.png");
	player1->sprite = Sprite::create(playerPath->getCString());
	PhysicsBody* playerBody = PhysicsBody::createBox(player1->sprite->getContentSize());
	playerBody->setRotationEnable(false);
	playerBody->getShape(0)->setRestitution(0.0f);
	playerBody->getShape(0)->setFriction(0.8f);
	playerBody->setMass(player1->mass);
	playerBody->setCollisionBitmask(0x04);
	playerBody->setCategoryBitmask(0x01);
	playerBody->setContactTestBitmask(0xFF);
	playerBody->getShape(0)->setTag(PLAYER_TAG);
	player1->sprite->setPhysicsBody(playerBody);
	player1->sprite->setPosition(player1->sprite->getContentSize().width/2+5, floor->getPositionY()+player1->sprite->getContentSize().height*3);
	addChild(player1->sprite,2,PLAYER_TAG);

	preLoadAnimation();

	auto backItem = MenuItemImage::create("images/CloseNormal.png", "images/CloseSelected.png", CC_CALLBACK_0(HelloWorld::toPopup, this));
	//auto pauseItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_0(HelloWorld::pauseScene, this));
	backItem->setPosition(backItem->getContentSize().width,0);

	auto backmenu = Menu::createWithItem(backItem);
	backmenu->setPosition(visibleSize.width - backItem->getContentSize().width*2, visibleSize.height - backItem->getContentSize().height / 2);
	addChild(backmenu, 10);

	hpLayer = HPLayer::create();
	hpLayer->setPosition(0,0);
	addChild(hpLayer, 2, HPLAYER_TAG);

	for (int i = 0; i < ATTACKNUMBER; i++){
		attackSprite[i] = Sprite::create();
	}
	//player1->sprite->addChild(attackSprite[1], 1);
	//this->addChild(attackSprite[2], 6);

	MenuItemSprite* attackLogoItem;
	for (int i = 0; i < ATTACKNUMBER; i++){
		auto logoPath = String::createWithFormat("%s%d%s%d%s", "images/level", level, "/attack", i, "/logo.png");
		auto normalSprite = Sprite::create(logoPath->getCString());
		normalSprite->setOpacity(100);
		auto selectedSprite = Sprite::create(logoPath->getCString());
		attackLogoItem = MenuItemSprite::create(normalSprite, selectedSprite, CC_CALLBACK_0(HelloWorld::changeAttack,this,i));
		attackLogoItem->setPosition(Vec2(0, -i*attackLogoItem->getContentSize().height));
		

		auto attackProgress = ProgressTimer::create(selectedSprite);
		attackProgress->setType(ProgressTimerType::RADIAL);
		attackProgress->setPercentage(0.0f);
		attackProgress->setPosition(attackLogoItem->getContentSize().width/2,attackLogoItem->getContentSize().height/2);
		attackLogoItem->addChild(attackProgress,1, ATTACKLOGO_TAG);

		attackLogoItems.pushBack(attackLogoItem);

	}
	auto attackLogoMenu = Menu::createWithArray(attackLogoItems);
	attackLogoMenu->setPosition(visibleSize.width - attackLogoItem->getContentSize().width, visibleSize.height / 2 + 2 * attackLogoItem->getContentSize().height);
	addChild(attackLogoMenu, 10);

	onTouchEvent();
	onKeyBoardEvent();
	physicsEvent();
	EventCustom e("custom");
	dispatcher->dispatchEvent(&e);

	//enemyComing(1.0f);
	
	this->schedule(schedule_selector(HelloWorld::enemyComing), 3.0f);
	this->schedule(schedule_selector(HelloWorld::enemyAttack), 2.0f);
	this->scheduleUpdate();
	
	log("init");
    return true;
}

//void HelloWorld :: onEnter(){
//	log("Enter");
	//this->schedule(schedule_selector(HelloWorld::enemyComing), 5.0f);
	//this->schedule(schedule_selector(HelloWorld::enemyAttack), 2.0f);
	//this->scheduleUpdate();
//}

//void HelloWorld::onExit(){
	//this->unscheduleAllSelectors();
//}


void HelloWorld::preLoadMusic(){
	auto bgMusicPath = String::createWithFormat("%s%d%s", "sounds/bgMusic_", level, ".mp3");
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bgMusicPath->getCString());
	
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/effect0.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("sounds/effect1.wav");

	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1f);
	SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
}
void HelloWorld::playBgMusic(){
	auto bgMusicPath = String::createWithFormat("%s%d%s", "sounds/bgMusic_", level, ".mp3");
	SimpleAudioEngine::getInstance()->playBackgroundMusic(bgMusicPath->getCString(), true);
}
void HelloWorld::pauseBgMusic(){
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void HelloWorld::playerEffectMusic(int i){
	auto effectPath = String::createWithFormat("%s%d%s","sounds/effect",i,".wav");
	SimpleAudioEngine::getInstance()->playEffect(effectPath->getCString(),false);
}

void HelloWorld::setEnemyFrameNumber(){
	enemyFrameNumber[0][0] = 8;
	enemyFrameNumber[0][1] = 4;
	enemyFrameNumber[0][2] = 6;
	enemyFrameNumber[0][3] = 15;

	enemyFrameNumber[1][0] = 8;
	enemyFrameNumber[1][1] = 4;
	enemyFrameNumber[1][2] = 6;
	enemyFrameNumber[1][3] = 15;

	bossFrameNumber[0][0] = 3;
	bossFrameNumber[0][1] = 3;
	bossFrameNumber[0][2] = 3;

	bossActionNumber[0][0] = 4;
	bossActionNumber[0][1] = 4;

	bossFrameNumber[1][0] = 3;
	bossFrameNumber[1][1] = 3;
	bossFrameNumber[1][2] = 3;

	bossActionNumber[1][0] = 4;
	bossActionNumber[1][1] = 4;
}
void HelloWorld::setPlayerFrameNumber(){
	playerFrameNumber[0][0] = 0;
	playerFrameNumber[0][1] = 12;
	playerFrameNumber[0][2] = 7;

	playerActionNumber[0][0] = 8;
	playerActionNumber[0][1] = 8;
	playerActionNumber[0][2] = 8;
	playerActionNumber[0][3] = 11;
	playerActionNumber[0][4] = 7;

	playerFrameNumber[1][0] = 0;
	playerFrameNumber[1][1] = 12;
	playerFrameNumber[1][2] = 7;

	playerActionNumber[1][0] = 8;
	playerActionNumber[1][1] = 8;
	playerActionNumber[1][2] = 8;
	playerActionNumber[1][3] = 11;
	playerActionNumber[1][4] = 7;
}

void HelloWorld::initDamage(){
	playerDamage.push_back(51.0f);
	playerDamage.push_back(201.0f);
	playerDamage.push_back(201.0f);
	for (int i = 0; i < ATTACKNUMBER; i++){
		attackColding[i] = 0.0f;
		attacking[i] = false;
	}

	attackColded[0] = 1;
	attackColded[1] = 5;
	attackColded[2] = 10;

	for (int i = 0; i < BOSS_ATTACKNUMBER; i++){
		bossColding[i] = i;
	}
	bossColded[0] = 10.0f;
	bossColded[1] = 10.0f;
	bossColded[2] = 10.0f;

	enemyDamage.push_back(21);
	enemyDamage.push_back(21);
	enemyDamage.push_back(21);
	enemyDamage.push_back(21);
	
}

void HelloWorld::preLoadAnimation(){
	for (int i = 0; i < ATTACKNUMBER; i++){
		playerAttackFrame[i].reserve(playerFrameNumber[level][i]);
		for (int j = 0; j < playerFrameNumber[level][i]; j++){
			auto attackPath = String::createWithFormat("%s%d%s%d%s%d%s", "images/level", level, "/attack",i,"/", j, ".png");
			auto tempPlayerSprite = Sprite::create(attackPath->getCString());
			playerAttackFrame[i].pushBack(SpriteFrame::create(attackPath->getCString(), Rect(0, 0, tempPlayerSprite->getContentSize().width, tempPlayerSprite->getContentSize().height)));
		}
	}

	for (int i = 0; i < ACTIONNUMBER; i++){
		playerActionFrame[i].reserve(playerActionNumber[level][i]);
		for (int j = 0; j < playerActionNumber[level][i]; j++){
			auto actionPath = String::createWithFormat("%s%d%s%d%s%d%s", "images/level", level, "/action", i, "/", j, ".png");
			auto tempPlayerSprite = Sprite::create(actionPath->getCString());
			playerActionFrame[i].pushBack(SpriteFrame::create(actionPath->getCString(), Rect(0, 0, tempPlayerSprite->getContentSize().width, tempPlayerSprite->getContentSize().height)));
		}
	}

	//enemy
	for (int i = 0; i < ENEMY_ATTACKNUMBER; i++){
		enemyAttackFrame[i].reserve(enemyFrameNumber[level][i]);
		for (int j = 0; j < enemyFrameNumber[level][i]; j++){
			auto enemyPath = String::createWithFormat("%s%d%s%d%s%d%s", "images/level", level, "/enemy", i, "/",j,".png");
			auto tempEnemySprite = Sprite::create(enemyPath->getCString());
			enemyAttackFrame[i].pushBack(SpriteFrame::create(enemyPath->getCString(), Rect(0, 0, tempEnemySprite->getContentSize().width, tempEnemySprite->getContentSize().height)));
		}
	}

	//boss
	for (int i = 0; i < BOSS_ATTACKNUMBER; i++){
		bossAttackFrame[i].reserve(bossFrameNumber[level][i]);
		for (int j = 0; j < bossFrameNumber[level][i]; j++){
			auto attackPath = String::createWithFormat("%s%d%s%d%s%d%s", "images/level", level, "/boss/attack", i, "/", j, ".png");
			auto tempPlayerSprite = Sprite::create(attackPath->getCString());
			bossAttackFrame[i].pushBack(SpriteFrame::create(attackPath->getCString(), Rect(0, 0, tempPlayerSprite->getContentSize().width, tempPlayerSprite->getContentSize().height)));
		}
	}
	
	for (int i = 0; i < BOSS_ACTIONNUMBER; i++){
		bossActionFrame[i].reserve(bossActionNumber[level][i]);
		for (int j = 0; j < bossActionNumber[level][i]; j++){
			auto actionPath = String::createWithFormat("%s%d%s%d%s%d%s", "images/level", level, "/boss/action", i, "/", j, ".png");
			auto tempPlayerSprite = Sprite::create(actionPath->getCString());
			bossActionFrame[i].pushBack(SpriteFrame::create(actionPath->getCString(), Rect(0, 0, tempPlayerSprite->getContentSize().width, tempPlayerSprite->getContentSize().height)));
		}
	}

}

void HelloWorld::playerParticle(String* plist, String* texture, Point position){
	ParticleSystemQuad* particle = ParticleSystemQuad::create(plist->getCString());
	particle->setTexture(TextureCache::sharedTextureCache()->addImage(texture->getCString()));
	log(plist->getCString());
	log(texture->getCString());
	particle->setDuration(0.1f);
	particle->setAutoRemoveOnFinish(true);
	auto pos = map1->convertToNodeSpace(position);
	particle->setPosition(pos);
	map1->addChild(particle, 10);
}

void HelloWorld::enemyComing(float dt){
	int num = (int)(abs(CCRANDOM_0_1() * 2-0.1)+1);
	if (bossComed) num -= 1;
	//if (mapEnd == true) num += 1;
	for (int i = 0; i < num; i++){
		int enemyType = (int)(CCRANDOM_0_1() * ENEMY_ATTACKNUMBER);
		//int enemyType = 1;
		enemy* enemy1 = new enemy();
		auto enemyPath = String::createWithFormat("%s%d%s%d%s", "images/level", level, "/enemy", enemyType, "/0.png");
		enemy1->sprite = Sprite::create(enemyPath->getCString());
		enemy1->attack = enemyType;
		if (enemy1->attack == 0) enemy1->moveSpeed *= 2;
		enemy1->distance = CCRANDOM_0_1() + 3;
		auto enemyBody = PhysicsBody::createBox(enemy1->sprite->getContentSize());
		enemyBody->setCategoryBitmask(0x01);
		enemyBody->setCollisionBitmask(0x04);
		enemyBody->setContactTestBitmask(0xFF);
		enemyBody->getShape(0)->setMass(10000);
		enemyBody->getShape(0)->setRestitution(0.0f);
		enemyBody->getShape(0)->setTag(ENEMY_TAG_ZERO + enemyType);
		enemy1->sprite->setPhysicsBody(enemyBody);
		addChild(enemy1->sprite, 2, ENEMY_TAG_ZERO + enemyType);
		auto tempSize = enemy1->sprite->getContentSize();
		
		if(mapEnd == false) enemy1->sprite->setPosition(visibleSize.width - enemy1->sprite->getContentSize().width / 2, visibleSize.height / 6 + enemy1->sprite->getContentSize().height / 2 + 5);
		else {
			if (CCRANDOM_0_1() <= 0.5f) enemy1->sprite->setPosition(visibleSize.width - enemy1->sprite->getContentSize().width / 2, visibleSize.height / 6 + enemy1->sprite->getContentSize().height / 2 + 5);
			else { 
				if(enemy1->attack != 3) enemy1->sprite->setPosition(enemy1->sprite->getContentSize().width / 2, visibleSize.height / 6 + enemy1->sprite->getContentSize().height / 2 + 5);
				else {
					enemy1->sprite->setPosition(CCRANDOM_0_1()*(visibleSize.width-2*enemy1->sprite->getContentSize().width)+enemy1->sprite->getContentSize().width,
						visibleSize.height / 6 + enemy1->sprite->getContentSize().height / 2 + 5);
				}
			}
		}
		if (enemy1->attack != 3){
			auto tempAnimate = Animate::create(Animation::createWithSpriteFrames(enemyAttackFrame[enemy1->attack], 0.1f));
			enemy1->sprite->runAction(RepeatForever::create(tempAnimate));
		}

		enemies.push_back(enemy1);
	}
}

bullet* HelloWorld::throwBullet(Point startPosition, Vec2 force){
	auto bulletPath = String::createWithFormat("%s%d%s", "images/level", level, "/bullet.png");
	auto bullet1 = new bullet();
	bullet1->sprite = Sprite::create(bulletPath->getCString());
	bullet1->sprite->setPosition(startPosition);
	auto bulletBody = PhysicsBody::createBox(bullet1->sprite->getContentSize());
	bulletBody->getShape(0)->setMass(bullet1->mass);
	bulletBody->getShape(0)->setRestitution(0.0f);
	bulletBody->setCategoryBitmask(0x00);
	bulletBody->setCollisionBitmask(0x00);
	bulletBody->setContactTestBitmask(0xFF);
	bulletBody->getShape(0)->setTag(BULLET_TAG);
	bulletBody->applyImpulse(force);
	bullet1->sprite->setPhysicsBody(bulletBody);
	return bullet1;
}

void HelloWorld::updatePlayerHP(float dt){
	auto HPProgess = (ProgressTimer*)(hpLayer->getChildByTag(HPTAG));
	float decreaseHP = player1->contactNumber * enemyDamage.at(2)* dt + 
								player1->beHitNumber* enemyDamage.at(1) + player1->beExploreNumber*enemyDamage.at(0);
	player1->HP -= decreaseHP;
	HPProgess->setPercentage(player1->HP);
	player1->beHitNumber = 0;
	player1->beExploreNumber = 0;

	if (player1->HP<= 0){
		gameOver = true;
		toPopup();
	}
}

void HelloWorld::changeAttack(int i){
	auto progress = (ProgressTimer*)(attackLogoItems.at(i)->getChildByTag(ATTACKLOGO_TAG));
	if (progress->getPercentage() >= 99.0f) {
		player1->setAttack(i);
		progress->setPercentage(0.0f);
	}
}

void HelloWorld::playerMove(bool isPressedW, bool isPressedA, bool isPressedD, float time){
	if (isPressedW && jumped == false){
			auto _jumpForce = Vec2(0, player1->jumpForce);
			player1->sprite->getPhysicsBody()->applyImpulse(_jumpForce);
			jumped = true;
		}
	else if (isPressedD) {
		if (player1->sprite->isFlippedX() && !player1->isAttacking) player1->sprite->setFlipX(false);
		if (player1->isRunning == false && player1->isAttacking == false){
			player1->sprite->stopAllActions();
			player1->isStanding= false;

			auto action = Sequence::create(Animate::create(Animation::createWithSpriteFrames(playerActionFrame[1], 0.1f)), CallFunc::create(CC_CALLBACK_0(player::clearRunning, player1)), NULL);
			player1->isRunning = true;
			player1->sprite->runAction(action);
		}

		if (map1->getPositionX() + map1->getContentSize().width / 2 < visibleSize.width + 6) mapEnd = true;

		if (player1->sprite->getPositionX() >= visibleSize.width / 2  && map1->getPositionX()+map1->getContentSize().width/2 >= visibleSize.width+5){
			map1->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			map2->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			for (auto tempEnemy : enemies){
				tempEnemy->sprite->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			}
			for (auto tempTool : tools){
				if(tempTool->isEffect == false) tempTool->sprite->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			}
			for (auto tempbullet : enemyBullets){
				tempbullet->sprite->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			}
			for (auto tempbullet : bullets){
				tempbullet->sprite->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
			}
		}
		else{
			if(player1->sprite->getPositionX() + player1->moveSpeed*time <= visibleSize.width - player1->sprite->getContentSize().width / 2 - 5) 
					player1->sprite->runAction(MoveBy::create(time, Vec2(player1->moveSpeed * time, 0)));
		}

	}
	else if (isPressedA){
		if (!player1->sprite->isFlippedX() && !player1->isAttacking) player1->sprite->setFlipX(true);
		if (player1->isRunning == false && player1->isAttacking == false){
			player1->sprite->stopAllActions();
			player1->isStanding = false;
			auto action = Sequence::create(Animate::create(Animation::createWithSpriteFrames(playerActionFrame[1], 0.1f)), CallFunc::create(CC_CALLBACK_0(player::clearRunning, player1)), NULL);
			player1->isRunning = true;
			player1->sprite->runAction(action);
		}
		if (player1->sprite->getPositionX() - player1->moveSpeed*time >= player1->sprite->getContentSize().width / 2 + 5) player1->sprite->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));
		//if (player1->isStanding == false && player1->isAttacking == false){
		//	player1->sprite->stopAllActions();
		//	player1->isRunning = false;
		//	auto action = Sequence::create(Animate::create(Animation::createWithSpriteFrames(playerActionFrame[0], 0.1f)), CallFunc::create(CC_CALLBACK_0(player::clearStanding, player1)), NULL);
		//	player1->isStanding = true;
		//	player1->sprite->runAction(action);
		//}
		//if (player1->sprite->getPositionX() - player1->moveSpeed*time >= player1->sprite->getContentSize().width / 2 + 5) player1->sprite->runAction(MoveBy::create(time, Vec2(-player1->moveSpeed * time, 0)));

	}
	else{
		if (player1->isStanding == false && player1->isAttacking == false){
			player1->sprite->stopAllActions();
			player1->isRunning = false;

			auto action = Sequence::create(Animate::create(Animation::createWithSpriteFrames(playerActionFrame[0], 0.1f)), CallFunc::create(CC_CALLBACK_0(player::clearStanding, player1)), NULL);
			player1->isStanding = true;
			player1->sprite->runAction(action);
			
		}
	}

}


void HelloWorld::enemyMove(float time){
	if (bossComed && boss->isAttacking == false && boss->isStanding == false){
		boss->isStanding = true;
		auto animation1 = Animation::createWithSpriteFrames(bossActionFrame[1], 0.3f);
		auto animate1 = Animate::create(animation1);
		boss->sprite->runAction(RepeatForever::create(animate1));

	}

	auto playerPos = player1->sprite->getPosition();
	auto playerSize = player1->sprite->getContentSize();
	std::vector<enemy*>::iterator enemyIt = enemies.begin();
	for (; enemyIt != enemies.end(); enemyIt++){
		(*enemyIt)->enemyMove(playerPos,visibleSize,playerSize,time);
	}
}

void HelloWorld::enemyAttack(float time){
	std::vector<enemy*>::iterator it = enemies.begin();
	for (; it != enemies.end(); it++){
		if ( (*it)->attack==1 && (*it)->isAttacking == false){
			(*it)->isAttacking = true;
			auto animation = Animation::createWithSpriteFrames(enemyAttackFrame[(*it)->attack], 0.1f);
			auto animate = Animate::create(animation);
			auto actions = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(enemy::clearAttack, (*it))), NULL);
			(*it)->sprite->runAction(actions);
			auto force = (*it)->impulse*(Vec2((player1->sprite->getPositionX() - (*it)->sprite->getPositionX()), visibleSize.height));
			auto tempBullet = throwBullet((*it)->sprite->getPosition(),force);
			this->addChild(tempBullet->sprite,3,ENEMY_BULLET_TAG);
			enemyBullets.push_back(tempBullet);
		}
	}
}

void HelloWorld::updateBullet(float time){
	int size = bullets.size();
	std::vector<bullet*>::iterator it;
	for (it = bullets.begin(); it != bullets.end();){
		Sprite* tempBullet = (*it)->sprite;
		bool needRemoveBullet = false;
		Vec2 tempPosition = tempBullet->getPosition();
		if (tempBullet->getPositionX() <= tempBullet->getContentSize().width / 2 + 1) needRemoveBullet = true;
		else if (tempBullet->getPositionX() >= visibleSize.width - tempBullet->getContentSize().width / 2 - 1) needRemoveBullet = true;
		else if (tempBullet->getPositionY() <= tempBullet->getContentSize().height / 2 + 1) needRemoveBullet = true;
		else if (tempBullet->getPositionY() >= visibleSize.height - tempBullet->getContentSize().height / 2 - 1) needRemoveBullet = true;

		std::vector<enemy*>::iterator enemyIt;
		for (enemyIt = enemies.begin(); enemyIt != enemies.end();){
			Sprite* tempEnemy = (*enemyIt)->sprite;
			if (abs(tempEnemy->getPositionX() - tempBullet->getPositionX()) <= tempEnemy->getContentSize().width / 2 + tempBullet->getContentSize().width / 2
				&& abs(tempEnemy->getPositionY() - tempBullet->getPositionY()) <= tempEnemy->getContentSize().height / 2 + tempBullet->getContentSize().height / 2){
				(*enemyIt)->HP -= playerDamage.at(0);
				needRemoveBullet = true;
				break;
			}
			else{
				enemyIt++;
			}
		}
		if (bossComed){
			Sprite* tempEnemy = boss->sprite;
			if (abs(tempEnemy->getPositionX() - tempBullet->getPositionX()) <= tempEnemy->getContentSize().width / 2 + tempBullet->getContentSize().width / 2
				&& abs(tempEnemy->getPositionY() - tempBullet->getPositionY()) <= tempEnemy->getContentSize().height / 2 + tempBullet->getContentSize().height / 2){
				boss->HP -= playerDamage.at(0);
				needRemoveBullet = true;
			}
		}

		if (needRemoveBullet){
			playerParticle( String::create("images/particle/particle1.plist"), String::create("images/particle/spiral.png"), tempPosition);
			playerEffectMusic(0);
			tempBullet->removeFromParentAndCleanup(true);
			it = bullets.erase(it);/////////////////////////////////////////////remeber
		}
		else{
			it++;
		}
	}
}

void HelloWorld::updateEnemyBullet(float time){
	int size = enemyBullets.size();
	std::vector<bullet*>::iterator it;
	for (it = enemyBullets.begin(); it !=enemyBullets.end();){
		Sprite* tempBullet = (*it)->sprite;
		bool needRemoveBullet = false;
		Vec2 tempPosition = tempBullet->getPosition();
		if (tempPosition.x<= tempBullet->getContentSize().width / 2 + 1) needRemoveBullet = true;
		else if (tempPosition.x>= visibleSize.width - tempBullet->getContentSize().width / 2 - 1) needRemoveBullet = true;
		else if (tempPosition.y<= tempBullet->getContentSize().height / 2 + 1) needRemoveBullet = true;
		else if (tempPosition.y >= visibleSize.height - tempBullet->getContentSize().height / 2 - 1) needRemoveBullet = true;
		else if (abs(player1->sprite->getPositionX() - tempPosition.x) <= (player1->sprite->getContentSize().width / 2 + tempBullet->getContentSize().width / 2) &&
			abs(player1->sprite->getPositionY() - tempPosition.y) <= (player1->sprite->getContentSize().height / 2 + tempBullet->getContentSize().height / 2)){
			if(!(player1->isAttacking&&player1->attack==1)) player1->beHitNumber++;
			needRemoveBullet = true;
		}

		if (needRemoveBullet){
			tempBullet->removeFromParentAndCleanup(true);
			it = enemyBullets.erase(it);/////////////////////////////////////////////remeber
		}
		else{
			it++;
		}
	}
}

void HelloWorld::updateEnemyAttack(float time){

	std::vector<enemy*>::iterator it = enemies.begin();
	for (; it != enemies.end(); it++){
		auto enemyPos = (*it)->sprite->getPosition();
		if ((*it)->attack == 3 && (*it)->isDying == 0 && abs(enemyPos.x - player1->sprite->getPositionX()) <= (*it)->sprite->getContentSize().width / 2 + player1->sprite->getContentSize().width / 2
			&& abs(enemyPos.y - player1->sprite->getPositionY()) <= (*it)->sprite->getContentSize().height / 2 + player1->sprite->getContentSize().height / 2){
			(*it)->isDying = 1;
			(*it)->HP = -1;
			auto tempEnemy = (*it);
			auto seq3 = Sequence::create(Animate::create(Animation::createWithSpriteFrames(enemyAttackFrame[tempEnemy->attack], 0.1f)),
				CallFunc::create(CC_CALLBACK_0(enemy::canEnemyRemove,tempEnemy)),NULL);
			tempEnemy->sprite->runAction(seq3);
		}
		else if ((*it)->attack == 0 && (*it)->isDying == 1){
			auto tempEnemy = (*it);
			auto seq0 = Sequence::create(TintTo::create(1.5f, Color3B::RED),
				CallFunc::create(CC_CALLBACK_0(enemy::canEnemyRemove, tempEnemy)),NULL);
			tempEnemy->sprite->runAction(seq0);
		}
	}
}

void HelloWorld::updatePlayerAttackOne(float time){
	if (bossComed){
		for (int i = 1; i < ATTACKNUMBER; i++){
			if (attacking[i]){
				auto tempPos = attackSprite[i]->getPosition();
				if (i == 1) tempPos = player1->sprite->getPosition();
				if (abs(boss->sprite->getPositionX() - tempPos.x) <= playerAttackFrame[i].at(0)->getRect().size.width / 2 + boss->sprite->getContentSize().width / 2 &&
					abs(boss->sprite->getPositionY() - tempPos.y) <= playerAttackFrame[i].at(0)->getRect().size.height / 2 + boss->sprite->getContentSize().height / 2){
					boss->HP -= playerDamage.at(i) * time;
				}
			}
		}
	}

	std::vector<enemy*>::iterator it = enemies.begin();
	for (; it != enemies.end();it++){
		/*
		for (int i = 1; i <= ATTACKNUMBER; i++){
			auto tempPos = attackSprite[i]->getPosition();
			if (abs((*it)->sprite->getPositionX() - tempPos.x) <= playerAttackFrame[index].at(0)->getRect().size.width/2 + (*it)->sprite->getContentSize().width/2 &&
				abs((*it)->sprite->getPositionY() - tempPos.y) <= playerAttackFrame[index].at(0)->getRect().size.height/2 + (*it)->sprite->getContentSize().height/2){
				(*it)->HP -= playerDamage.at(index) * time;
			}
		}
		*/
		for (int i = 1; i < ATTACKNUMBER; i++){
			if (attacking[i]){
				String* str = String::createWithFormat("%d",i);
				log(str->getCString());
				auto tempPos = attackSprite[i]->getPosition();
				if (i == 1) tempPos = player1->sprite->getPosition();
				if (abs((*it)->sprite->getPositionX() - tempPos.x) <= playerAttackFrame[i].at(0)->getRect().size.width / 2 + (*it)->sprite->getContentSize().width / 2 &&
					abs((*it)->sprite->getPositionY() - tempPos.y) <= playerAttackFrame[i].at(0)->getRect().size.height / 2 + (*it)->sprite->getContentSize().height / 2){
					(*it)->HP -= playerDamage.at(i) * time;
				}
			}
		}
	}
}

void HelloWorld::setReadyNext(){
	readyNextStage = true;
}

void HelloWorld::updateEnemyHP(float time){
	if (bossComed && boss->HP <= 0.0f ){
		boss->HP = 99999;
		boss->sprite->stopAllActions();
		boss->isStanding = true;
		auto animation = Animation::createWithSpriteFrames(bossActionFrame[0], 0.5f);
		auto animate = Animate::create(animation);
		auto actions = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(HelloWorld::setReadyNext,this)),NULL);
		boss->sprite->runAction(actions);
	}

	std::vector<enemy*>::iterator enemyIt = enemies.begin();
	for (; enemyIt != enemies.end();){

		//if ((*enemyIt)->sprite->getPositionX() <= -visibleSize.width/2+(*enemyIt)->sprite->getContentSize().width/2){
		//	(*enemyIt)->sprite->removeFromParentAndCleanup(true);
		//	enemyIt = enemies.erase(enemyIt);
	 //		continue;
	//	}

		if ((*enemyIt)->HP <= 0){
			bool canAddTool = false;
			Point toolPos = Point(0,0);
			if ((*enemyIt)->attack == 0 ){
				if ((*enemyIt)->isDying == 0) {
					(*enemyIt)->isDying = 1;
					enemyIt++; 
				}
				else if ((*enemyIt)->isDying == 2){
					auto tempPos = (*enemyIt)->sprite->getPosition();
					(*enemyIt)->sprite->removeFromParentAndCleanup(true);
					enemyIt = enemies.erase(enemyIt);
					playerEffectMusic(1);
					canAddTool = true;
					toolPos = tempPos;
					playerParticle(String::create("images/particle/particle1.plist"),String::create("images/particle/scull.png"),tempPos);
					auto tempSprite = Sprite::create("images/particle/scull.png");
					if (abs(player1->sprite->getPositionX() - tempPos.x) <= player1->sprite->getContentSize().width + tempSprite->getContentSize().width &&
						abs(player1->sprite->getPositionY() - tempPos.y) <= player1->sprite->getContentSize().width + tempSprite->getContentSize().height){
						player1->beExploreNumber++;
					}
				}
				else{
					enemyIt++;
				}
			}

			else if ((*enemyIt)->attack == 3){
				if ((*enemyIt)->isDying == 2){
					auto tempPos = (*enemyIt)->sprite->getPosition();
					(*enemyIt)->sprite->removeFromParentAndCleanup(true);
					enemyIt = enemies.erase(enemyIt);
					canAddTool = true;
					toolPos = tempPos;
					playerEffectMusic(1);
					playerParticle(String::create("images/particle/particle1.plist"), String::create("images/particle/scull.png"), tempPos);
					auto tempSprite = Sprite::create("images/particle/scull.png");
					if (abs(player1->sprite->getPositionX() - tempPos.x) <= player1->sprite->getContentSize().width + tempSprite->getContentSize().width &&
						abs(player1->sprite->getPositionY() - tempPos.y) <= player1->sprite->getContentSize().width + tempSprite->getContentSize().height){
						player1->beExploreNumber++;
					}
				}
				else{
					enemyIt++;
				}
			}

			else{
				canAddTool = true;
				toolPos = (*enemyIt)->sprite->getPosition();
				(*enemyIt)->sprite->removeFromParentAndCleanup(true);
				enemyIt = enemies.erase(enemyIt);
			}
			if (canAddTool){
				toolPos.y = visibleSize.height / 6 + player1->sprite->getContentSize().height/2;
				addTools(toolPos);
				auto scoreLabel = (Label*)(hpLayer->getChildByTag(SCORETAG));
				auto score = scoreLabel->getString();
				score[5]+=1;
				for (int i = 5; i >= 0; i--){
					if (score[i] > '9') {
						score[i] = '0';
						if(i-1>=0) score[i - 1] ++;
						else{
							gameOver = true;
							toPopup();
						}
					}
				}
				scoreLabel->setString(score);
			}
		}
		else{
			enemyIt++;
		}
	}
}


void HelloWorld::bossComing(){
	bossComed = true;
	boss = new player();
	auto temp = String::createWithFormat("%s%d%s","images/level",level,"/boss/action1/0.png");
	boss->sprite = Sprite::create(temp->getCString());
	if(boss->sprite->isFlippedX() == false) boss->sprite->setFlipX(true);
	boss->HP = 500.0f;
	boss->sprite->setPosition(visibleSize.width - boss->sprite->getContentSize().width, visibleSize.height / 6 + boss->sprite->getContentSize().height / 2 + 5);
	this->addChild(boss->sprite,2);
	auto animation = Animation::createWithSpriteFrames(bossActionFrame[1], 0.3f);
	auto animate = Animate::create(animation);
	boss->sprite->runAction(RepeatForever::create(animate));
	boss->isStanding = true;
}

void HelloWorld::bossAttack(int x){

	if (x == 1 && boss->isAttacking == false){
		boss->isAttacking = true;
		boss->sprite->stopAllActions();

		for (int i = 0; i < 3; i++){
			int enemyType = 3;
			enemy* enemy1 = new enemy();
			auto enemyPath = String::createWithFormat("%s%d%s%d%s", "images/level", level, "/enemy", enemyType, "/0.png");
			enemy1->sprite = Sprite::create(enemyPath->getCString());
			enemy1->attack = enemyType;
			if (enemy1->attack == 0) enemy1->moveSpeed *= 2;
			enemy1->distance = CCRANDOM_0_1() + 3;
			auto enemyBody = PhysicsBody::createBox(enemy1->sprite->getContentSize());
			enemyBody->setCategoryBitmask(0x01);
			enemyBody->setCollisionBitmask(0x04);
			enemyBody->setContactTestBitmask(0xFF);
			enemyBody->getShape(0)->setMass(10000);
			enemyBody->getShape(0)->setRestitution(0.0f);
			enemyBody->getShape(0)->setTag(ENEMY_TAG_ZERO + enemyType);
			enemy1->sprite->setPhysicsBody(enemyBody);
			addChild(enemy1->sprite, 2, ENEMY_TAG_ZERO + enemyType);
			enemy1->sprite->setPosition(CCRANDOM_0_1()*(visibleSize.width - 2 * enemy1->sprite->getContentSize().width) + enemy1->sprite->getContentSize().width,
				visibleSize.height / 6 + enemy1->sprite->getContentSize().height / 2 + 5);
			enemies.push_back(enemy1);
		}
		
		auto animation = Animation::createWithSpriteFrames(bossAttackFrame[0], 0.3f);
		auto animate = Animate::create(animation);

		auto actions = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(player::clearAttack, boss)), NULL);
		boss->sprite->runAction(actions);
	}
}

void HelloWorld::update(float time){
	//循环地图
	Sprite* tempMap1 = (Sprite*)this->getChildByTag(MAP1_TAG);
	Sprite* tempMap2 = (Sprite*)this->getChildByTag(MAP2_TAG);
	if (tempMap1->getPositionX() + tempMap1->getContentSize().width / 2 <= origin.x){
		//tempMap1->setPositionX(tempMap2->getContentSize().width+tempMap1->getContentSize().width/2);
		float offset = tempMap1->getPositionX() + tempMap1->getContentSize().width / 2 - origin.x;
		tempMap1->setPosition(Point(tempMap1->getContentSize().width / 2 + origin.x + tempMap2->getContentSize().width + offset, visibleSize.height / 2 + origin.y));
	}
	if (tempMap2->getPositionX() + tempMap2->getContentSize().width / 2 <= origin.x) {
		//tempMap2->setPositionX(tempMap1->getContentSize().width+tempMap2->getContentSize().width/2);
		float offset = tempMap2->getPositionX() + tempMap2->getContentSize().width / 2 - origin.x;
		tempMap2->setPosition(Point(tempMap2->getContentSize().width / 2 + origin.x + tempMap1->getContentSize().width + offset, visibleSize.height / 2 + origin.y));
	}

	if (mapEnd && !nextStage){
		nextStage = true;
		if (stage == 1) bossComing();
		else if(stage == 0){
			auto str = String::createWithFormat("%s%d%s", "images/level", level, "/GO.png");
			auto nextSprite = Sprite::create(str->getCString());
			nextSprite->setOpacity(100.0f);
			auto clickSprite = Sprite::create(str->getCString());
			clickSprite->setOpacity(100.0f);

			goItem = MenuItemSprite::create(nextSprite, clickSprite);
			goItem->setPosition(0, 0);

			auto normalSprite = Sprite::create(str->getCString());
			goProgress = ProgressTimer::create(normalSprite);
			goProgress->setType(ProgressTimerType::BAR);
			goProgress->setMidpoint(Point(0, 0));
			goProgress->setPercentage(0.0f);
			goProgress->setBarChangeRate(Vec2(0, 1));
			goProgress->setPosition(goItem->getContentSize().width / 2, goItem->getContentSize().height / 2);

			goItem->addChild(goProgress, 1);

			Menu* goMenu = Menu::create(goItem, NULL);
			goMenu->setPosition(visibleSize.width / 2, visibleSize.height /3*2);
			this->addChild(goMenu, 10);
		}
	}
	if (nextStage){
		if (stage == 0){
			auto nowPercentage = goProgress->getPercentage();
			if (nowPercentage < 100.0f){
				float increase = time * 3.0f;
				goProgress->setPercentage(nowPercentage + increase);
			}
			else {
				this->unschedule(schedule_selector(HelloWorld::enemyComing));
				int number = 0;
				Vector<enemy*>::iterator it = enemies.begin();
				for (; it != enemies.end(); it++){
					if ((*it)->sprite->getPositionX() < visibleSize.width && (*it)->sprite->getPositionX() > 0) number++;
				}
				if (number == 0){
					endSave();
					stage = 1;
					goItem->setCallback(CC_CALLBACK_0(HelloWorld::restart, this));
				}
			}
		}
		else if (stage == 1){
			if (readyNextStage == true){
				endSave();
				stage = 0;
				level = 1;
				toPopup();
			}
		}
	}

	//playerMove
	playerMove(isPressedW, isPressedA, isPressedD, time);
	//enemy0Move
	enemyMove(time);

	//updateBullet
	updateBullet(time);

	//updateEnemyBullet
	updateEnemyBullet(time);

	updateTools(time);

	updateEnemyAttack(time);

	updatePlayerAttackOne(time);

	updatePlayerHP(time);

	updateEnemyHP(time);

	updateAttackColding(time);
}

void HelloWorld::endSave(){
	auto preLevel = 0;
	auto preStage = 0;
	auto maxScore = 0;
	if (database->getBoolForKey("isExit")){
		preLevel = database->getIntegerForKey("nowLevel");
		preStage = database->getIntegerForKey("nowStage");
		maxScore = database->getIntegerForKey("maxScore");
	}
	if (level > preLevel) {
		datebase->setIntegerForKey("nowLevel", level);
		datebase->setIntegerForKey("nowStage", stage);
	}
	else if (level == preLevel && stage > preStage){
		datebase->setIntegerForKey("nowLevel", level);
		datebase->setIntegerForKey("nowStage", stage);
	}

	auto scoreLabel = (Label*)(hpLayer->getChildByTag(SCORETAG));
	auto nowScore = scoreLabel->getString();
	int temp = 0;
	for (int i = 0; i < 3; i++){
		temp = temp * 10 + nowScore[i] - '0';
	}
	if (maxScore < temp)
		datebase->setIntegerForKey("maxScore", temp);
}

void HelloWorld::updateAttackColding(float time){
	for (int i = 0; i < ATTACKNUMBER; i++){
		auto progress = (ProgressTimer*)(attackLogoItems.at(i)->getChildByTag(ATTACKLOGO_TAG));
		float nowPercent = progress->getPercentage();
		if (nowPercent < 100) progress->setPercentage(nowPercent+=100/attackColded[i]*time);
	}

	if (bossComed){
		for (int i = 0; i < BOSS_ATTACKNUMBER; i++){
			bossColding[i] += bossColded[i]  *time/8;
			if (bossColding[i] >= bossColded[i]){
				boss->isStanding = false;
				bossAttack(i);
				bossColding[i] = 0.0f;
			}
		}
	}

}

void HelloWorld::addTools(Point pos){
	if (CCRANDOM_0_1() <= 0.3f){
		int type = (int)(CCRANDOM_0_1() * 2 - 0.1);
		auto tool1 = new tool(level, type);
		tool1->sprite->setPosition(pos);
		auto action = JumpTo::create(0.5f,pos,tool1->sprite->getContentSize().height,1);
		tool1->sprite->runAction(action);
		this->addChild(tool1->sprite, 6);
		tools.push_back(tool1);
	}
}

void HelloWorld::updateTools(float time){
	auto playerPos = player1->sprite->getPosition();
	std::vector<tool*>::iterator it = tools.begin();
	for (; it != tools.end(); ){
		if ((*it)->isEffect) {
			(*it)->lasted += time;
			if ((*it)->lasted >= 5.1f){
				recover((*it)->type);
				it = tools.erase(it);
				continue;
			}
		}
		else if(abs((*it)->sprite->getPosition().x - playerPos.x) <= player1->sprite->getContentSize().width / 2 + (*it)->sprite->getContentSize().width / 2 &&
			abs((*it)->sprite->getPosition().y - playerPos.y) <= player1->sprite->getContentSize().height / 2 + (*it)->sprite->getContentSize().height / 2){
			(*it)->sprite->removeFromParentAndCleanup(true);
			(*it)->effect(player1);
		}
		it++;
	}
}

void HelloWorld::recover(int type){
	if (type == 1){
		player1->moveSpeed = player1->normalMoveSpeed;
	}
}


void HelloWorld::onTouchEvent(){
	//触屏事件
	auto toucherListener = EventListenerTouchOneByOne::create();
	toucherListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	toucherListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	toucherListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	dispatcher->addEventListenerWithSceneGraphPriority(toucherListener, this);
}
void HelloWorld::onKeyBoardEvent(){
	//按键事件
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
}

void HelloWorld::customEvent(){
	//自定义事件
	auto customEventListener = EventListenerCustom::create("custom", [](EventCustom* e){
		log("'custom' event");
	});
	dispatcher->addEventListenerWithSceneGraphPriority(customEventListener,this);
}

void HelloWorld::physicsEvent(){
	//碰撞事件
	auto physicsListener = EventListenerPhysicsContact::create();
	physicsListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin,this);
	physicsListener->onContactSeperate = CC_CALLBACK_1(HelloWorld::onContactSeperate,this);
	dispatcher->addEventListenerWithSceneGraphPriority(physicsListener,this);
}

bool HelloWorld::onContactBegin(PhysicsContact& contact){
	//接触开始
	auto a = contact.getShapeA()->getTag();
	auto b = contact.getShapeB()->getTag();
	if ((a*b) == PLAYER_TAG * EDGE_TAG) {
		isContact = true;
		isPressedW = false;
		jumped = false;
	}
	else if ((a*b) == PLAYER_TAG*(ENEMY_TAG_ZERO+2)){
		player1->contactNumber++;
	}
	auto temp = String::createWithFormat("%d", player1->contactNumber);
	log(temp->getCString());
	log("onContactBegin");
	return true;
}

void HelloWorld::onContactSeperate(PhysicsContact& contact){
	//接触结束
	auto a = contact.getShapeA()->getTag();
	auto b = contact.getShapeB()->getTag();
	if ((a * b) == PLAYER_TAG * EDGE_TAG){
		isContact = false;
	}
	else if ((a*b) == PLAYER_TAG * (ENEMY_TAG_ZERO+2)){
		player1->contactNumber--;
	}
	log("onContactSeperate");
}

void HelloWorld::particleTest(){
	//粒子效果
	auto particle = ParticleExplosion::create();
	this->addChild(particle);
}

void HelloWorld::playerAttack(Point touchPos){
	if (touchPos.x > player1->sprite->getPositionX() && player1->sprite->isFlippedX()) player1->sprite->setFlipX(false);
	else if (touchPos.x <= player1->sprite->getPositionX() && !player1->sprite->isFlippedX()) player1->sprite->setFlipX(true);

	auto animation = Animation::createWithSpriteFrames(playerAttackFrame[player1->attack], 0.1f);
	auto animate = Animate::create(animation);
	//auto attack1 = Animate::create(Animation::createWithSpriteFrames(attack1Frame, 0.4f));

	//animateSprite->setPosition(player1->sprite->getContentSize().width / 2, player1->sprite->getContentSize().height / 2);
	//player1->sprite->addChild(animateSprite);

	if (player1->attack == 1){
		attacking[1] = true;
		attackSprite[1] = Sprite::create();
		player1->sprite->addChild(attackSprite[1],1);
		auto actions = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(player::clearAttack, player1)), CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, attackSprite[1],true)), 
									CallFunc::create(CC_CALLBACK_0(HelloWorld::eraseAnimateSprite,this,1)) ,NULL);
		attackSprite[1]->setPosition(player1->sprite->getContentSize().width / 2, player1->sprite->getContentSize().height / 2);
		attackSprite[1]->runAction(actions);
		player1->setAttack(0);
	}
	else if (player1->attack == 2){
		attacking[2] = true;
		attackSprite[2] = Sprite::create();
		this->addChild(attackSprite[2],6);
		auto actions = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(player::clearAttack, player1)), CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParentAndCleanup, attackSprite[2],true)),
									CallFunc::create(CC_CALLBACK_0(HelloWorld::eraseAnimateSprite, this, 2)), NULL);
		auto tempPos = Point(touchPos.x, visibleSize.height/3);
		player1->attackPoint[2] = tempPos;
		attackSprite[2]->setPosition(touchPos.x, visibleSize.height / 3);
		
		attackSprite[2]->runAction(actions);
		player1->setAttack(0);
		
	}

	else if (player1->attack == 0){
		auto animate0 = Animate::create(Animation::createWithSpriteFrames(playerActionFrame[4],0.1f));
		auto actions0 = Sequence::create(animate0, CallFunc::create(CC_CALLBACK_0(player::clearAttack, player1)), NULL);
		player1->sprite->runAction(actions0);
		auto force = player1->impulse*Vec2(touchPos.x - player1->sprite->getPositionX(), touchPos.y - player1->sprite->getPositionY());
		if (abs(force.x) > abs(force.y)) force.x *= 1.5;
		else force.y *= 1.5;
		bullet* bullet1 = throwBullet(player1->sprite->getPosition(), force);
		this->addChild(bullet1->sprite,3,BULLET_TAG);
		bullets.push_back(bullet1);
	}
}

void HelloWorld::eraseAnimateSprite(int attack){
	attacking[attack] = false;
}


bool HelloWorld::onTouchBegan(Touch* touch, Event* event){
	if (!player1->isAttacking){
		player1->isAttacking = true;
		player1->sprite->stopAllActions();
		player1->isRunning = false;
		player1->isStanding = false;
		playerAttack(touch->getLocation());
	}
	log("onTouchBegin");
	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event){
	log("onTouchMoved");
}
void HelloWorld::onTouchEnded(Touch* touch, Event* event){
	log("onTouchEnded");
}
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode code, Event* event){
	if (code == EventKeyboard::KeyCode::KEY_D){
		if (!isPressedD) {
			isPressedD = true;
		}
	}
	else if (code == EventKeyboard::KeyCode::KEY_A){
		if (!isPressedA) {
			isPressedA = true;
		}
	}
	else if (code == EventKeyboard::KeyCode::KEY_W){
		if (!isPressedW) {
			isPressedW = true;
		}
	}
	log("onKeyPressed");
}
void HelloWorld::onKeyReleased(EventKeyboard::KeyCode code, Event* event){
	if (code == EventKeyboard::KeyCode::KEY_D){
		if (isPressedD) isPressedD = false;
	}
	else if (code == EventKeyboard::KeyCode::KEY_A){
		if (isPressedA) isPressedA = false;
	}
	log("onKeyReleased");
}

//void HelloWorld::setLevel(Ref* pSender){
//	auto msg = (String*)pSender;
//	level = msg->intValue();
//	for (int i = 0; i < 100; i++){
//		log("okok");
//	}
//}

void HelloWorld::backToChoseLevel(){
	auto scene = ChoseLevel::createScene();
	Director::getInstance()->replaceScene(CCTransitionCrossFade::create(0.5f, scene));
	pauseBgMusic();
}

void HelloWorld::toPopup(){
	auto director = Director::getInstance();
	RenderTexture* renderTexture = RenderTexture::create(visibleSize.width,visibleSize.height);
	renderTexture->begin();
	this->getParent()->visit();
	renderTexture->end();
	
	auto scene = pausePopup::createScene(renderTexture, "images/popupBg.png");
	auto layer = pausePopup::create(renderTexture, "images/popupBg.png");
	scene->addChild(layer, 1);

	if (gameOver) {
		layer->contentText = Label::createWithTTF("游戏结束", "fonts/msyhbd.ttc", 30);
		layer->contentText->setPosition(layer->menuSprite->getContentSize().width / 2, layer->menuSprite->getContentSize().height / 3 * 2);
		layer->menuSprite->addChild(layer->contentText);

		auto restartLabel = Label::createWithTTF("重新开始", "fonts/msyhbd.ttc", 15);
		restartLabel->setColor(Color3B::BLACK);
		auto restartItem = MenuItemLabel::create(restartLabel, CC_CALLBACK_0(HelloWorld::restart,this));
		restartItem->setPosition(-restartItem->getContentSize().width, 0);

		auto levelLabel = Label::createWithTTF("选择关卡", "fonts/msyhbd.ttc", 15);
		levelLabel->setColor(Color3B::BLACK);
		auto levelItem = MenuItemLabel::create(levelLabel, CC_CALLBACK_0(pausePopup::backToLevel, layer));
		levelItem->setPosition(levelItem->getContentSize().width, 0);

		layer->menu = Menu::create(restartItem ,levelItem,NULL);
	}
	else if (readyNextStage){
		endSave();
		auto scoreLabel = (Label*)(hpLayer->getChildByTag(SCORETAG));
		auto nowScore = scoreLabel->getString();
		int temp = 0;
		for (int i = 0; i <= 5; i++){
			temp = temp * 10 + nowScore[i] - '0';
		}
		auto preMax = database->getIntegerForKey("maxScore");
		auto str = String::createWithFormat("%s%d%s%d","恭喜通关\n当前得分为: ",temp,"\n原纪录最高分为: ", preMax);
		layer->contentText = Label::createWithTTF(str->getCString(), "fonts/msyhbd.ttc", 15);
		layer->contentText->setPosition(layer->menuSprite->getContentSize().width / 2, layer->menuSprite->getContentSize().height / 3 * 2);
		layer->menuSprite->addChild(layer->contentText);
		auto backLabel = Label::createWithTTF("下一关", "fonts/msyhbd.ttc", 15);
		backLabel->setColor(Color3B::BLACK);
		auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_0(HelloWorld::restart, this));
		backItem->setPosition(backItem->getContentSize().width * 3 / 2, 0);

		auto levelLabel = Label::createWithTTF("选择关卡", "fonts/msyhbd.ttc", 15);
		levelLabel->setColor(Color3B::BLACK);
		auto levelItem = MenuItemLabel::create(levelLabel, CC_CALLBACK_0(pausePopup::backToLevel, layer));
		levelItem->setPosition(0, 0);

		auto overLabel = Label::createWithTTF("重新开始", "fonts/msyhbd.ttc", 15);
		overLabel->setColor(Color3B::BLACK);
		auto overItem = MenuItemLabel::create(overLabel, CC_CALLBACK_0(HelloWorld::restart, this));
		overItem->setPosition(-overItem->getContentSize().width * 3 / 2, 0);

		layer->menu = Menu::create(backItem, levelItem, overItem, NULL);
	}
	else{
		layer->contentText = Label::createWithTTF("游戏暂停", "fonts/msyhbd.ttc", 30);
		layer->contentText->setPosition(layer->menuSprite->getContentSize().width / 2, layer->menuSprite->getContentSize().height / 3 * 2);
		layer->menuSprite->addChild(layer->contentText);
		auto backLabel = Label::createWithTTF("返回游戏", "fonts/msyhbd.ttc", 15);
		backLabel->setColor(Color3B::BLACK);
		auto backItem = MenuItemLabel::create(backLabel, CC_CALLBACK_0(pausePopup::backToGame, layer));
		backItem->setPosition(backItem->getContentSize().width*3/2 , 0);

		auto levelLabel = Label::createWithTTF("选择关卡", "fonts/msyhbd.ttc", 15);
		levelLabel->setColor(Color3B::BLACK);
		auto levelItem = MenuItemLabel::create(levelLabel, CC_CALLBACK_0(pausePopup::backToLevel, layer));
		levelItem->setPosition(0, 0);

		auto overLabel = Label::createWithTTF("重新开始", "fonts/msyhbd.ttc", 15);
		overLabel->setColor(Color3B::BLACK);
		auto overItem = MenuItemLabel::create(overLabel, CC_CALLBACK_0(HelloWorld::restart, this));
		overItem->setPosition(-overItem->getContentSize().width*3/2, 0);

		layer->menu = Menu::create(backItem, levelItem, overItem, NULL);
	}

	layer->menu->setPosition(layer->menuSprite->getContentSize().width / 2,  layer->menuSprite->getContentSize().height /12);
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

void HelloWorld::pauseScene(){
	if (!isPaused) {
		Director::getInstance()->pause();
		isPaused = true;
	}
	else {
		Director::getInstance()->resume();
		isPaused = false;
	}
}

void HelloWorld::initPar(){
	isPaused = false;
	isPressedD = false;
	isPressedA = false;
	isPressedW = false;
	jumped = false;
	isContact = false;
	sceneIsPaused = false;
	grivity = 600;
	gameOver = false;

}

void HelloWorld::restart(){
	auto scene = HelloWorld::createScene();
	auto layer = HelloWorld::create(scene->getPhysicsWorld(), level, stage);
	scene->addChild(layer);
	Director::getInstance()->replaceScene(CCTransitionProgressInOut::create(0.5,scene));
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}




