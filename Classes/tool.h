#ifndef _TOOL_H_
#define _TOOL_H_

#include "cocos2d.h"
#include "player.h"

USING_NS_CC;

class tool{
public:
	Sprite* sprite;
	int type;
	int level;
	bool isEffect = false;
	float lasted;

	tool(int level, int type);
	void effect(player* player1);
	void recover(float time);

};

#endif