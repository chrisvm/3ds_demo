#include "bullet_manager.h"

BulletManager::BulletManager()
{
	m_bullets = new std::vector<BulletPosition*>();
	bullet_sprite = new Bullet();
	bullet_sprite->Load();
	bullet_sprite->origin.y = 0.25f;
	bullet_sprite->origin.x = 0.5f;
}

BulletManager::~BulletManager()
{
	std::vector<BulletPosition*>::iterator bullet_pos = m_bullets->begin();
	for (; bullet_pos != m_bullets->end(); bullet_pos++) {
		delete *bullet_pos;
	}
	delete m_bullets;
}

void BulletManager::CreateBullet(float x, float y, float rotation)
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
	vec2f movemnt;
	std::vector<BulletPosition*>::iterator bullet_pos = m_bullets->begin();
	for (; bullet_pos != m_bullets->end(); bullet_pos++) {
		movemnt.x = cos((*bullet_pos)->rotation - M_PI_2) * m_bullet_speed * delta_time;
		movemnt.y = sin((*bullet_pos)->rotation - M_PI_2) * m_bullet_speed * delta_time;
		(*bullet_pos)->pos += movemnt;
	}
}

void BulletManager::Draw(SceneContext* scene)
{
	BulletPosition* b_pos;
	std::vector<BulletPosition*>::iterator bullet_pos = m_bullets->begin();
	for (; bullet_pos != m_bullets->end(); bullet_pos++) {
		b_pos = *bullet_pos;
		bullet_sprite->pos = b_pos->pos;
		bullet_sprite->rotation = b_pos->rotation;
		bullet_sprite->Draw(scene);
	}
}
