#include "drawing.h"

void drawSpriteVBO(size_t idx, int x, int y, int width, int height, ImageDimension image, VBOEntry* vbo) {
	float left = image.left;
	float right = image.right;
	float top = image.top;
	float bottom = image.bottom;

	VBOEntry *entry = &vbo[idx * 6];

	*entry++ = (VBOEntry){ x,         y,          0.5f, left,  top    };
	*entry++ = (VBOEntry){ x,         y + height, 0.5f, left,  bottom };
	*entry++ = (VBOEntry){ x + width, y,          0.5f, right, top    };

	*entry++ = (VBOEntry){ x+width, y,        0.5f, right, top    };
	*entry++ = (VBOEntry){ x,       y+height, 0.5f, left,  bottom };
	*entry++ = (VBOEntry){ x+width, y+height, 0.5f, right, bottom };
}

void shiftEndianess(u8* src, u8* dst, unsigned width, unsigned height) {
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

void moveSprites(Sprite* sprites, int spriteCount) {
	for (int i = 0; i < spriteCount; i++) {
		sprites[i].x += sprites[i].dx;
		sprites[i].y += sprites[i].dy;

		//check for collision with the screen boundaries
		if (sprites[i].x < 1 || sprites[i].x > (400 - 32)) {
			sprites[i].dx = -sprites[i].dx;
        }

		if (sprites[i].y < 1 || sprites[i].y > (240 - 32)) {
			sprites[i].dy = -sprites[i].dy;
        }
	}
}

void drawSprites(SceneContext* scene, Sprite* sprites, int numSprites, ImageDimension imgDim) {
	// Update the uniforms
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, scene->uLoc_projection, &scene->projection);

	for (size_t i = 0; i < numSprites; i++) {
		drawSpriteVBO(i, sprites[i].x, sprites[i].y, 32, 32, imgDim, scene->vbo);
	}

    C3D_DrawArrays(GPU_TRIANGLES, 0, numSprites * 6);
}
