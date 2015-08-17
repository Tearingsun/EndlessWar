#include "tool.h"
USING_NS_CC;

tool::tool(int _level, int _type){
	level = _level;
	type = _type;
	auto imagePath = String::createWithFormat("%s%d%s%d%s", "images/level",level,"/tools/tool",type,".png");
	sprite = Sprite::create(imagePath->getCString());
	log(imagePath->getCString());
	lasted = 0.0f;
}

void tool::effect(player* player1){
	isEffect = true;
	if (type == 0){
		player1->HP += 50;
		if (player1->HP > 100.0f) player1->HP = 100.0f;
	}
	else if (type == 1){
		if(player1->moveSpeed == player1->normalMoveSpeed) player1->moveSpeed = 2*player1->normalMoveSpeed;
	}
}
