#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
#include "ChoseLevel.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "vector"
#include "HPLayer.h"
#include "pausePopup.h"
#include "tool.h"
USING_NS_CC;

#define MAP1_TAG 1
#define MAP2_TAG 2
#define PLAYER_TAG 5
#define FLOOR_TAG 3
#define EDGEONE_TAG 41
#define EDGE_TAG 40
#define BULLET_TAG 6
#define ENEMY_BULLET_TAG 66
#define ENEMY_TAG_ZERO 7
#define ENEMY_TAG_ONE 8
#define HPLAYER_TAG 9
#define ATTACKLOGO_TAG 13
#define BOSS_ATTACKNUMBER 3
#define BOSS_ACTIONNUMBER 2

#define datebase UserDefault::getInstance()

class HelloWorld : public cocos2d::Layer
{
public:
	
	Size winSize;
	Size visibleSize;
	Point origin;
	PhysicsWorld* m_world;
	EventDispatcher* dispatcher;

	player* player1;
	Sprite* map1;
	Sprite* map2;
	ActionInterval* action;
	ActionInterval* mapAction;

	player* boss;
	float bossColding[3];
	float bossColded[3];

	bool isPaused;

	bool isPressedD;
	bool isPressedA;
	bool isPressedW;
	bool jumped;
	bool isContact;

	bool sceneIsPaused;

	bool mapEnd = false;
	bool nextStage = false;
	bool readyNextStage = false;
	int stage = 0;

    static Scene* createScene();
	static HelloWorld* create(PhysicsWorld* world, int level, int stage);
    virtual bool init(PhysicsWorld* world,int level,int stage);
	void initPar();
	bool gameOver;
	
	ProgressTimer* goProgress = NULL;
	MenuItemSprite* goItem;
	void changeStage(int i);

	HPLayer* hpLayer;

	int level;
	float grivity;
	int finalScore = 0;

	std::vector<bullet*> bullets;
	std::vector<enemy*> enemies;
	std::vector<bullet*> enemyBullets;
	std::vector<tool*> tools;

	Vector<SpriteFrame*> enemyAttackFrame[ENEMY_ATTACKNUMBER];
	int enemyFrameNumber[LEVELNUMBER][ENEMY_ATTACKNUMBER];
	void setEnemyFrameNumber();

	Vector<SpriteFrame*> playerAttackFrame[ATTACKNUMBER];
	int playerFrameNumber[LEVELNUMBER][ATTACKNUMBER];

	Vector<SpriteFrame*> playerActionFrame[ACTIONNUMBER];
	int playerActionNumber[LEVELNUMBER][ACTIONNUMBER];

	Vector<SpriteFrame*> bossActionFrame[BOSS_ACTIONNUMBER];
	int bossActionNumber[LEVELNUMBER][BOSS_ACTIONNUMBER];

	Vector<SpriteFrame*> bossAttackFrame[BOSS_ATTACKNUMBER];
	int bossFrameNumber[LEVELNUMBER][BOSS_ATTACKNUMBER];

	double attackColding[ATTACKNUMBER];
	double attackColded[ATTACKNUMBER];

	Sprite* attackSprite[ATTACKNUMBER];
	void eraseAnimateSprite(int attack);
	bool attacking[ATTACKNUMBER];

	Vector<MenuItem*> attackLogoItems;

	void setPlayerFrameNumber();
	void endSave();

	void initDamage();
	std::vector<float> playerDamage;
	std::vector<float> enemyDamage;
	//void setPlayerDamage(int x, float damage);
	//void setEnemyDamage(int x, float damage);

    void menuCloseCallback(cocos2d::Ref* pSender);
	void backToChoseLevel();
	void toPopup();

	void preLoadMusic();
	void playBgMusic();
	void pauseBgMusic();
	void playerEffectMusic(int i);
	void playerParticle(String* plist, String* texture, Point position);

	void preLoadAnimation();

	void enemyComing(float dt);
	
	void setReadyNext();

	bool bossComed = false;
	void bossComing();
	void bossAttack(int x);
	
	bullet* throwBullet(Point startPosition,Vec2 force);
	void playerMove(bool isPressedW, bool isPressedA, bool isPressedD,float time);
	void playerAttack(Point touchPos);
	void enemyMove(float time);
	void enemyAttack(float time);
	void particleMove(float time);

	void addTools(Point position);
	void recover(int type);

	void update(float time);
	void updatePlayerHP(float dt);
	void updateEnemyHP(float dt);
	void updateBullet(float time);
	void updateEnemyBullet(float time);
	void updatePlayerAttackOne(float time);
	void updateEnemyAttack(float time);
	void updateTools(float time);
	void updateAttackColding(float time);
	
	void changeAttack(int i);

	void onTouchEvent();
	void onKeyBoardEvent();
	void customEvent();
	void physicsEvent();

	void particleTest();
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	bool onContactBegin(PhysicsContact& contact);
	void onContactSeperate(PhysicsContact& contact);

	void pauseScene();

	void restart();

	//virtual void onExit();
	//virtual void onEnter();

};

#endif // __HELLOWORLD_SCENE_H__
