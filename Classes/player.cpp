#include "player.h"

void player::setAttack(int _attack){
	attack = _attack;
}

void player::clearAttack(){
	isAttacking = false;
	log("okokok");
}

player::player(){
	normalMoveSpeed = 200.0f;
	moveSpeed = normalMoveSpeed;
	mass = 10000;
	jumpForce = 200 * 20000;
	impulse = 50;
	attack = 0;
	isAttacking = false;
	HP = 100.0f;
}

void player::clearRunning(){
	isRunning = false;
}

void player::clearStanding(){
	isStanding = false;
}
