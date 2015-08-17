#include "cocos2d.h"
USING_NS_CC;

#define HPTAG 10
#define SCORETAG 11

class HPLayer : public Layer{
public:
	virtual bool init();
	ProgressTimer* HPProgress;
	Label* score;
	CREATE_FUNC(HPLayer);
};