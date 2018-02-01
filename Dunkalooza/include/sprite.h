#ifndef _SPRITE_H_
#define _SPRITE_H_
#include <citro3d.h>
#include "types.h"

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

	C3D_Mtx* CalculateModelMatrix();
    void WriteToVBO(VBOEntry* vbo, int startingIndex);

private:
	C3D_Mtx model;
};

#endif
