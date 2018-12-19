#pragma once
#include "cGameObject.h"
class cPickUp :
	public cGameObject
{
public:
	cPickUp();
	void update(double deltaTime);
	~cPickUp();
};

