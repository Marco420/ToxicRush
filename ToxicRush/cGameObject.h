/*
=================
cGameObject.h
- Header file for class definition - SPECIFICATION
- Header file for the player class which is a child of cSprite class
=================
*/
#ifndef _CGAMEOBJECT_
#define _CGAMEOBJECT_
#include "cSprite.h"
#include <random>

class cGameObject : public cSprite
{
private:

	random_device rd;    // non-deterministic engine 
	mt19937 gen{ rd() };
	LPCSTR objectName = "";
	

public:
	int objectSpeed = 425;
	LPCSTR textureName = "";

	cGameObject();
	cGameObject(LPCSTR textureName);
	LPCSTR getObjectName();
	virtual void Initalise();	//void render();		// Default render function
	virtual void update(double deltaTime);	
	~cGameObject();
	
	};
#endif