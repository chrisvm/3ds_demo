#ifndef _SHIP_H_
#define _SHIP_H_
#include "sprite.h"
#include "spaceship_png.h"
#include "types.h"
#include "drawing.h"
#include "lodepng.h"

class Ship : public Sprite {

public:
	void LoadImage();
};

#endif