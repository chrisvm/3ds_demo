#ifndef _TYPES_H_
#define _TYPES_H_

#define CLEAR_COLOR 0x000000FF

// Used to transfer the final rendered display to the framebuffer
#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

// Used to convert textures to 3DS tiled format
// Note: vertical flip flag set so 0,0 is top left of texture
#define TEXTURE_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(1) | GX_TRANSFER_OUT_TILED(1) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define MAX_SPRITES   1600
#define MAX_IMMEDIATE 500
#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

//simple sprite struct
typedef struct {
    float x,y;			// screen co-ordinates
    float dx, dy;			// velocity
    int image;
} Sprite;

//VBO entry
typedef struct {
    float x, y, z;
    float u, v;
} VBOEntry;

typedef struct {
    float left, right, top, bottom;
} ImageDimension;

#endif //INC_3D_BREW_TYPES_H
