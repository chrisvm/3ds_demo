#ifndef __BULLET_MANAGER_H_
#define __BULLET_MANAGER_H_
#include "bullet.h"
#include <vector>

struct BulletPosition
{
	vec2f pos;
	float rotation;

	BulletPosition(vec2f pos, float rotation)
	{
		this->pos = pos;
		this->rotation = rotation;
	}
	BulletPosition(float x, float y, float rotation)
	{
		this->pos.x = x;
		this->pos.y = y;
		this->rotation = rotation;
	}
};

class BulletManager
{
	std::vector<BulletPosition*> *m_bullets;
	Bullet* bullet_sprite;
	float m_bullet_speed = 150;
	vec2f screen_size;

public:
	BulletManager();
	~BulletManager();

	void CreateBullet(float x, float y, float rotation);
	void WriteToVBO(VBOEntry* vbo);
	void Update(float delta_time);
	void Draw(SceneContext* scene);
	void SetScreenSize(vec2f size);
};


#endif
