#include "drawing.h"

void drawSprite(SceneContext* scene, Sprite* sprite)
{
	// create rotation matrix
	C3D_FVec rotAxis = FVec4_New(sprite->x, sprite->y, 0.5f, 1.0f);
	//todo: create rotation matrix to send to gpu uniform
	
	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, scene->uLoc_projection, &scene->projection);

	drawSpriteVBO(sprite, scene->vbo);

    C3D_DrawArrays(GPU_TRIANGLES, 0, 6);
}

void drawSpriteVBO(Sprite* sprite, VBOEntry* vbo)
{
	float x = sprite->x;
	float y = sprite->y;
	float width = sprite->width;
	float height = sprite->height;
	float rotation = sprite->rotation;

	ImageDimension image = sprite->imageDimension;
	float left = image.left;
	float right = image.right;
	float top = image.top;
	float bottom = image.bottom;

	vbo[0] = (VBOEntry){ x,         y,          0.5f, left,  top    };
	vbo[1] = (VBOEntry){ x,         y + height, 0.5f, left,  bottom };
	vbo[2] = (VBOEntry){ x + width, y,          0.5f, right, top    };

	vbo[3] = (VBOEntry){ x + width, y,          0.5f, right, top    };
	vbo[4] = (VBOEntry){ x,         y + height, 0.5f, left,  bottom };
	vbo[5] = (VBOEntry){ x + width, y + height, 0.5f, right, bottom };

	// apply rotation matrix
	float sinTheta = sin(rotation), cosTheta = cos(rotation);
	float xOrigTrans = (sprite->x + sprite->width / 2);
	float yOrigTrans = (sprite->y + sprite->height / 2);
	for (int index = 0; index < 6; index++) {
		x = vbo[index].x - xOrigTrans;
		y = vbo[index].y - yOrigTrans;
		vbo[index].x = (cosTheta * x) - (sinTheta * y) + xOrigTrans;
		vbo[index].y = (sinTheta * x) + (cosTheta * y) + yOrigTrans;
	}
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
