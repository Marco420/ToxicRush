#ifndef _CENEMY_H
#define _CENEMY_H
#include "cGameObject.h"
class cEnemy : public cGameObject 
{
private :
	

public:
	bool Shoot();
	cEnemy();
	void Initialise();
	~cEnemy();
};
#endif


