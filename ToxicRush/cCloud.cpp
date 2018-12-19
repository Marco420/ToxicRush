#include "cCloud.h"



cCloud::cCloud()
{
}

void cCloud::Update(double deltaTime)
{
	
	SDL_Rect currentPos = getSpritePos();
	setSpriteTranslation({ objectSpeed,0 });
	currentPos.x -= (getSpriteTranslation().x * deltaTime);

	setSpritePos({ currentPos.x,currentPos.y });

	if (getSpritePos().x < -getSpriteDimensions().w)
	{
		setSpritePos({ currentPos.x + cloudDisX(gen) ,cloudDisY(gen) });
	}


}


cCloud::~cCloud()
{
}
