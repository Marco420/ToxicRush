#include "cEnemy.h"



cEnemy::cEnemy()
{
}

void cEnemy::Initialise()
{
	
}


cEnemy::~cEnemy()
{
}

bool cEnemy::Shoot()
{
	if (getSpritePos().x < WINDOW_WIDTH)
	{
		return true;
	}
}
 