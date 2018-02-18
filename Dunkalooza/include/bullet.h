#ifndef _BULLETS_H_
#define _BULLETS_H_
#include "sprite.h"
#include "bullet_png.h"

class Bullet : public Sprite {
public:
	void Load();
	void Update(float deltaTime);
};
#endif
