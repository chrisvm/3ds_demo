#include "bullet_manager.h"

bool between(vec2f point, vec2f min, vec2f max)
{
	if (point.x < min.x || point.y < min.y) return false;
	if (point.x > max.x || point.y > max.y) return false;
	return true;
}

BulletManager::BulletManager()
{
	m_bullets = new std::list<BulletPosition*>();
	bullet_sprite = new Bullet();
	bullet_sprite->Load();
	bullet_sprite->origin.y = 0.25f;
	bullet_sprite->origin.x = 0.5f;
}

BulletManager::~BulletManager()
{
	std::list<BulletPosition*>::iterator bullet_pos = m_bullets->begin();
	for (; bullet_pos != m_bullets->end(); bullet_pos++) {
		delete *bullet_pos;
	}
	delete m_bullets;
}

void BulletManager::CreateBullet(float x, float y, C3D_FQuat rotation)
{
	BulletPosition* bullet_pos = new BulletPosition(x, y, rotation);
	m_bullets->push_back(bullet_pos);
}

void BulletManager::WriteToVBO(VBOEntry* vbo)
{
	bullet_sprite->WriteToVBO(vbo);
}

void BulletManager::Update(float delta_time)
{
	if (m_bullets->size() == 0) return;

	vec2f movemnt;
	std::list<BulletPosition*>::iterator bullet_pos = m_bullets->begin();
	for (; bullet_pos != m_bullets->end(); bullet_pos++) {
		if (!between((*bullet_pos)->pos, vec2f(0, 0), m_screen_size)) {
			bullet_pos = m_bullets->erase(bullet_pos);
			if (bullet_pos == m_bullets->end()) continue;
		}

		//movemnt.x = cos((*bullet_pos)->rotation - M_PI_2) * m_bullet_speed * delta_time;
		//movemnt.y = sin((*bullet_pos)->rotation - M_PI_2) * m_bullet_speed * delta_time;
		(*bullet_pos)->pos += movemnt;
	}
}

void BulletManager::Draw(SceneContext* scene)
{
	BulletPosition* b_pos;
	std::list<BulletPosition*>::iterator bullet_pos = m_bullets->begin();
	for (; bullet_pos != m_bullets->end(); bullet_pos++) {
		b_pos = *bullet_pos;
		bullet_sprite->pos = b_pos->pos;
		bullet_sprite->rotation = b_pos->rotation;
		bullet_sprite->Draw(scene);
	}
}

void BulletManager::SetScreenSize(vec2f size)
{
	m_screen_size = size;
}
