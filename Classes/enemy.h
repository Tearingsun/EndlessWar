
#include "cocos2d.h"
#include "bullet.h"
USING_NS_CC;

#define LEVELNUMBER 2
#define ENEMY_ATTACKNUMBER 4

class enemy{
public:
	Sprite* sprite;
	float moveSpeed;
	float mass;
	float jumpForce;
	float impulse;
	int attack;
	bool isAttacking;
	int isDying;

	float beHitNumber = 0;

	float distance; ///enemy1

	float HP = 100.0f;

	enemy();
	void setAttack(int _attack);
	void clearAttack();
	void canEnemyRemove();

	void enemyMove(Point playerPos, Size visibleSize, Size playerSize, float time);
};
