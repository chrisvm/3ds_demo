#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <citro3d.h>
#include "types.h"
#include "lodepng.h"
#include "scene_context.h"

//simple sprite struct
class Sprite {

public:
    // screen co-ordinates
    float x, y;

    // screen width and height
    float width, height;

    // velocity
    float dx, dy;

    // sprite origin
    float origin_x, origin_y;

	// ratation support
	float rotation, ang_vel;

    // image dimension
    ImageDimension imageDimension;

    // gpu texture
    C3D_Tex spritesheet_tex;

	C3D_Mtx* CalculateModelMatrix();
    void WriteToVBO(VBOEntry* vbo);
    void BindToTextureUnit(int unitIndex);
    void Draw(SceneContext* scene);
    void MoveToFacing(float speed);

protected:
    void LoadImage(const unsigned char* in, size_t size);
    int vboIndex;

private:
	C3D_Mtx model;

    void ShiftEndianess(u8* src, u8* dst, unsigned width, unsigned height);
};

#endif
