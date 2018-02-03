#ifndef _BULLETS_H_
#define _BULLETS_H_
#include "sprite.h"
#include "bullet_png.h"

class Bullets : public Sprite {
public:
	void Load();
	void Update();
};
#endif
