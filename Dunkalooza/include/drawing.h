#ifndef _DRAWING_H_
#define _DRAWING_H_
#include <3ds.h>
#include <citro3d.h>
#include "types.h"
#include "sprite.h"
#include "scene_context.h"

void drawSprite(SceneContext* scene, Sprite* sprite);
void drawSpriteToVBO(Sprite* sprite, VBOEntry* vbo);
void shiftEndianess(u8* src, u8* dst, unsigned width, unsigned height);

#endif
