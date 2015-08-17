#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "cocos2d.h"
USING_NS_CC;

#define ATTACKNUMBER 3
#define ACTIONNUMBER 5


class player {
public:
	Sprite* sprite;
	float moveSpeed;
	float mass;
	float jumpForce;
	float impulse;
	int attack = 0;
	float HP;
	bool isAttacking = false;
	bool jumped = false;
	float normalMoveSpeed = 200.0f;

	int beHitNumber = 0;
	int beExploreNumber = 0;

	int contactNumber = 0;
	
	player();
	void setAttack(int _attack);
	void clearAttack();
	
	bool isRunning = false;
	void clearRunning();
	bool isStanding = false;
	void clearStanding();

	Point attackPoint[ATTACKNUMBER];

};
#endif