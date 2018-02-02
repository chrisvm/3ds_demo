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

void Sprite::WriteToVBO(VBOEntry* vbo, int startingIndex)
{
	float left = imageDimension.left;
	float right = imageDimension.right;
	float top = imageDimension.top;
	float bottom = imageDimension.bottom;

	vbo[startingIndex + 0] = (VBOEntry){ 0, 0, 0.5f, left, top };
	vbo[startingIndex + 1] = (VBOEntry){ 0, height, 0.5f, left, bottom };
	vbo[startingIndex + 2] = (VBOEntry){ width, 0, 0.5f, right, top };

	vbo[startingIndex + 3] = (VBOEntry){ width, 0, 0.5f, right, top };
	vbo[startingIndex + 4] = (VBOEntry){ 0, height, 0.5f, left, bottom };
	vbo[startingIndex + 5] = (VBOEntry){ width, height, 0.5f, right, bottom };
}

void Sprite::BindToTextureUnit(int unitIndex) 
{
	C3D_TexBind(unitIndex, &spritesheet_tex);
}