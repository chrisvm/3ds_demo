#include "bullets.h"

void Bullets::Load()
{
	LoadImage(bullet_png, bullet_png_size);
	imageDimension = {0.0f, 1.0f, 0.0f, 1.0f};
	vboIndex = 1;
    this->width = 16;
    this->height = 32;
}

void Bullets::Update()
{

}
