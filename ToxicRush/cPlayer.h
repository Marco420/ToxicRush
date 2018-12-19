/*
=================
cPlayer.h
- Header file for class definition - SPECIFICATION
- Header file for the player class which is a child of cSprite class
=================
*/
#ifndef _CPLAYER_H
#define _CPLAYER_H
#include "cSprite.h"

class cPlayer : public cSprite
{
private:
	int playerJumpSpeed = 7;
	double playerRotAngle;
	int initialPos;
	int playerJumpHeight = 180;
	bool isDead = false;
	LPCSTR textureName = "player";
	LPCSTR textureName1 = "playerGun";
	
	
	
	
	

public:
	
	bool playerGun;
	bool ground = true;
	bool isJumping = false;
	bool crouch = false;
	bool withGun = false;
	LPCSTR getPlayerName();
	void initialise();
	void addGravity();
	//void render();		// Default render function
	void Update(double deltaTime);		// Rocket update method
	void setPlayerRotation(double theRotAngle);   // Sets the rotation angle for the player
	double getPlayerRotation();	
	int getPlayerJumpHeight();
	bool playerJumping();
	void Jump();
	void Die();

	
};

#endif