#include "drawing.h"

void drawSprite(SceneContext* scene, Sprite* sprite)
{
	// Update the uniforms
	C3D_Mtx *model = sprite->CalculateModelMatrix();
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, scene->uLoc_model, model);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, scene->uLoc_projection, &scene->projection);

	drawSpriteToVBO(sprite, scene->vbo);

    C3D_DrawArrays(GPU_TRIANGLES, 0, 6);
}

void drawSpriteToVBO(Sprite* sprite, VBOEntry* vbo)
{
	float width = sprite->width;
	float height = sprite->height;

	ImageDimension image = sprite->imageDimension;
	float left = image.left;
	float right = image.right;
	float top = image.top;
	float bottom = image.bottom;

	vbo[0] = (VBOEntry){ 0, 0, 0.5f, left, top };
	vbo[1] = (VBOEntry){ 0, height, 0.5f, left, bottom };
	vbo[2] = (VBOEntry){ width, 0, 0.5f, right, top };

	vbo[3] = (VBOEntry){ width, 0, 0.5f, right, top };
	vbo[4] = (VBOEntry){ 0, height, 0.5f, left, bottom };
	vbo[5] = (VBOEntry){ width, height, 0.5f, right, bottom };
}

void shiftEndianess(u8* src, u8* dst, unsigned width, unsigned height)
{
    // lodepng outputs big endian rgba so we need to convert
    for (uint i = 0; i < width * height; i++) {
        int r = *src++;
        int g = *src++;
        int b = *src++;
        int a = *src++;

        *dst++ = a;
        *dst++ = b;
        *dst++ = g;
        *dst++ = r;
    }
}
