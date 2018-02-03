#include "ship.h"

void Ship::Load()
{
	LoadImage(spaceship_png, spaceship_png_size);
	vboIndex = 0;
    imageDimension = {0.0f, 1.0f, 0.0f, 1.0f};
    this->width = 32;
    this->height = 32;
}
