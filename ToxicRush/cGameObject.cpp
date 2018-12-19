#include "cGameObject.h"



cGameObject::cGameObject()
{
	
}

cGameObject::cGameObject(LPCSTR textureName)
{
	textureName = "";
}

LPCSTR getTextureName(LPCSTR textureName)
{
	return textureName;
}
void cGameObject::Initalise()
{
}



 void cGameObject::update(double deltaTime)
{
	//Make GameObjects move
	SDL_Rect currentPos = getSpritePos();
	setSpriteTranslation({ objectSpeed,0 });
	currentPos.x -= (getSpriteTranslation().x * deltaTime);
	setSpritePos({ currentPos.x,currentPos.y });

	if (getSpritePos().x < 0 - getSpriteCentre().x * 2)
	{
		setSpritePos({ currentPos.x +1024 ,50});
	}
	this->setSpritePos({ currentPos.x, currentPos.y });


	//Set Collider Around GameObject
	this->setBoundingRect(this->getSpritePos());

}

LPCSTR cGameObject::getObjectName()
{
	return objectName;
}




cGameObject::~cGameObject()
{
}


