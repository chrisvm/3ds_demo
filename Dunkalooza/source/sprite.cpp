#include "sprite.h"

C3D_Mtx* Sprite::CalculateModelMatrix()
{
	Mtx_Identity(&model);
	Mtx_Translate(&model, this->x, this->y, 0, true);
	Mtx_RotateZ(&model, rotation, true);
	float x_trans = -this->origin_x * this->width;
	float y_trans = -this->origin_y * this->height;
	Mtx_Translate(&model, x_trans, y_trans, 0, true);
	return &model;
}
