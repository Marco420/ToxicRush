#include "cPlayerBullet.h"



cPlayerBullet::cPlayerBullet()
{
}

void cPlayerBullet::update(double deltaTime)
{
	this->setSpritePos({ this->getSpritePos().x + 10,this->getSpritePos().y });
	this->setBoundingRect(this->getSpritePos());
}


cPlayerBullet::~cPlayerBullet()
{
}
