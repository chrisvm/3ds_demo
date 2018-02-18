#ifndef __BULLET_MANAGER_H_
#define __BULLET_MANAGER_H_
#include "bullet.h"
#include <vector>

struct BulletPosition
{
	float x, y;
};

class BulletManager
{
	std::vector<BulletPosition> *m_bullets;

public:
	BulletManager();
	~BulletManager();
	void CreateBullet(float x, float y);
};


#endif
