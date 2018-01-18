#ifndef _DRAWING_H_
#define _DRAWING_H_
#include <3ds.h>
#include <citro3d.h>
#include "types.h"
#include "scene_context.h"

void drawSpriteVBO(size_t idx, int x, int y, int width, int height, int image, VBOEntry* vbo);
void shiftEndianess(u8* src, u8* dst, unsigned width, unsigned height);
void moveSprites(Sprite* sprites, int spriteCount);
void drawSprites(SceneContext* scene, Sprite* sprites, int numSprites, ImageDimension imgDim);

#endif
