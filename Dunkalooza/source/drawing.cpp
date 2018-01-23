#include "drawing.h"

void drawSprites(SceneContext* scene, Sprite* sprites)
{
	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, scene->uLoc_projection, &scene->projection);

	drawSpriteVBO(sprites->x, sprites->y, 148, 148, sprites->imageDimension, scene->vbo);

    C3D_DrawArrays(GPU_TRIANGLES, 0, 6);
}

void drawSpriteVBO(int x, int y, int width, int height, ImageDimension image, VBOEntry* vbo)
{
	float left = image.left;
	float right = image.right;
	float top = image.top;
	float bottom = image.bottom;

	*vbo++ = (VBOEntry){ x,         y,          0.5f, left,  top    };
	*vbo++ = (VBOEntry){ x,         y + height, 0.5f, left,  bottom };
	*vbo++ = (VBOEntry){ x + width, y,          0.5f, right, top    };

	*vbo++ = (VBOEntry){ x+width, y,        0.5f, right, top    };
	*vbo++ = (VBOEntry){ x,       y+height, 0.5f, left,  bottom };
	*vbo++ = (VBOEntry){ x+width, y+height, 0.5f, right, bottom };
}

void shiftEndianess(u8* src, u8* dst, unsigned width, unsigned height)
{
    // GX_DisplayTransfer needs input buffer in linear RAM

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

void moveSprites(Sprite* sprites, int spriteCount)
{
	for (int i = 0; i < spriteCount; i++) {
		sprites[i].x += sprites[i].dx;
		sprites[i].y += sprites[i].dy;

		//check for collision with the screen boundaries
		if (sprites[i].x < 1 || sprites[i].x > (400 - 148)) {
			sprites[i].dx = -sprites[i].dx;
        }

		if (sprites[i].y < 1 || sprites[i].y > (240 - 148)) {
			sprites[i].dy = -sprites[i].dy;
        }
	}
}
