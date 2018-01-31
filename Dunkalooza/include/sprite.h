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

	// ratation support
	float rotation, ang_vel;

    // image dimension
    ImageDimension imageDimension;

    Sprite();
	C3D_Mtx* CalculateModelMatrix();

private:
    C3D_FVec rotationAxis;
	C3D_Mtx model;
};

#endif
