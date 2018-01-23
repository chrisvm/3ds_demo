#ifndef _DRAWING_H_
#define _DRAWING_H_
#include <3ds.h>
#include <citro3d.h>
#include "types.h"
#include "scene_context.h"

void drawSprites(SceneContext* scene, Sprite* sprite);
void drawSpriteVBO(int x, int y, int width, int height, ImageDimension image, VBOEntry* vbo);
void shiftEndianess(u8* src, u8* dst, unsigned width, unsigned height);
void moveSprites(Sprite* sprites);

#endif
