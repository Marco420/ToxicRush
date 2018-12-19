#pragma once
#include "cGameObject.h"
#include <random>
class cCloud :
	public cGameObject
{
private:
	
	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() };
	uniform_int_distribution<> cloudDisY{ -100,50 };
	uniform_int_distribution<> cloudDisX{2500,3500 };

public:
	 
	LPCSTR textureName = { "cloud" };

	cCloud();
	virtual void Update(double deltaTime);
	~cCloud();
};

