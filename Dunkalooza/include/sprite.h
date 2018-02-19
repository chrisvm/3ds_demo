#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <citro3d.h>
#include "types.h"
#include "lodepng.h"
#include "vec2.h"
#include "scene_context.h"

//simple sprite struct
class Sprite
{

public:
    // screen co-ordinates
    vec2f pos;

    // screen width and height
    float width, height;

    // velocity
    vec2f vel;

    // sprite origin
    vec2f origin;

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
