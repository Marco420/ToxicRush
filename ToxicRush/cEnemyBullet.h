#pragma once
#include "cGameObject.h"
class cEnemyBullet :
	public cGameObject
{
public:
	void update(double deltaTime);
	cEnemyBullet();
	~cEnemyBullet();
};

