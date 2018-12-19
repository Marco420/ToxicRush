#include "cEnemyBullet.h"



cEnemyBullet::cEnemyBullet()
{
}

void cEnemyBullet::update(double deltaTime)
{
	this->setSpritePos({ this->getSpritePos().x - 10,this->getSpritePos().y });
	this->setBoundingRect(this->getSpritePos());
}

cEnemyBullet::~cEnemyBullet()
{
}
