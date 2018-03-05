#ifndef __BULLET_MANAGER_H_
#define __BULLET_MANAGER_H_
#include "bullet.h"
#include <list>

struct BulletPosition
{
	vec2f pos;
	C3D_FQuat rotation;

	BulletPosition(vec2f pos, C3D_FQuat rotation)
	{
		this->pos = pos;
		this->rotation = rotation;
	}
	BulletPosition(float x, float y, C3D_FQuat rotation)
	{
		this->pos.x = x;
		this->pos.y = y;
		this->rotation = rotation;
	}
};

class BulletManager
{
	std::list<BulletPosition*> *m_bullets;
	Bullet* bullet_sprite;
	float m_bullet_speed = 150;
	vec2f m_screen_size;

public:
	BulletManager();
	~BulletManager();

	void CreateBullet(float x, float y, C3D_FQuat rotation);
	void WriteToVBO(VBOEntry* vbo);
	void Update(float delta_time);
	void Draw(SceneContext* scene);
	void SetScreenSize(vec2f size);
};


#endif
