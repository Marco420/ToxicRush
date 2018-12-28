/*
=================
cPlayer.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPlayer.h"

/*
=================================================================
Initialise the sprite variables
=================================================================
*/





void cPlayer::initialise()
{
	
	
}

void cPlayer::addGravity()
{
	if (this->getSpritePos().y < GROUND && !isJumping && !ground)
	{
		this->setSpritePos({ this->getSpritePos().x  ,this->getSpritePos().y + playerJumpSpeed });
	}
	else 	{
		this->ground = true;
	}
}

void cPlayer::Update(double deltaTime)
{
	if (withGun)
	{
		textureName = "playerGun";
	}
	else
	{
		textureName = "player";
	}
	
	
	if (isJumping)
	{
		setSpritePos({ getSpritePos().x  ,getSpritePos().y - playerJumpSpeed });
	}
	if (getSpritePos().y < initialPos - getPlayerJumpHeight())
	{
		isJumping = false;
	}
	
	if (!ground  && !isJumping)
	{
		addGravity();
	}
	
	this->setBoundingRect(this->getSpritePos());
	
	
}

LPCSTR cPlayer::getPlayerName()
{
	if (playerGun)
	{
		return textureName1;
	}
	else
	{
		return textureName;

	}
}


void cPlayer::Jump()
{
	ground = false;
	initialPos = getSpritePos().y;
 	isJumping = true;

	

	 
}

void cPlayer::Die()
{
	
 
	
}



/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
//void cPlayer::setPlayerSpeed(int playerSpeed)
//{
//	playerSpeed = playerSpeed;
//}
/*
=================================================================
Gets the rocket velocity
=================================================================
*/
//int cPlayer::getPlayerSpeed()
//{
//	return playerSpeed;
//}
int cPlayer::getPlayerJumpHeight()
{
	return playerJumpHeight;
}

bool cPlayer::playerJumping()
{
	return isJumping;
}
/*
=================================================================
Sets the rotation angle for the rocket
=================================================================
*/
void cPlayer::setPlayerRotation(double theRotAngle)
{
	//playerRotAngle = theRotAngle;
}
/*
=================================================================
Gets the rotation angle for the rocket
=================================================================
*/
double cPlayer::getPlayerRotation()
{
	return playerRotAngle;
}
