#include "bullet_manager.h"

BulletManager::BulletManager()
{
	m_bullets = new std::vector<Bullet>();
}

BulletManager::~BulletManager()
{
	delete m_bullets;
}

void BulletManager::CreateBullet(float x, float y)
{

}
