#pragma once
#include "cGameObject.h"
class cPlayerBullet :
	public cGameObject
{
public:
	void update(double deltaTime);
cPlayerBullet();
	~cPlayerBullet();
};

