#include "sprite.h"

Sprite::Sprite()
{
	rotationAxis = FVec4_New(this->x, this->y, 0.5f, 1.0f);
}

C3D_Mtx* Sprite::CalculateModelMatrix()
{
	rotationAxis.x = this->x;
	rotationAxis.y = this->y;
	//Mtx_Rotate(&model, rotationAxis, rotation, true);
	Mtx_Identity(&model);
	Mtx_Translate(&model, -this->x, -this->y, 0, true);
	Mtx_RotateZ(&model, rotation, true);
	Mtx_Translate(&model, this->x, this->y, 0, true);
	return &model;
}
