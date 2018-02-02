#include "ship.h"

void Ship::LoadImage() 
{
	unsigned char* image;
    unsigned width, height;

    lodepng_decode32(&image, &width, &height, spaceship_png, spaceship_png_size);

    u8 *gpusrc = (u8*) linearAlloc(width * height * 4);
    shiftEndianess(image, gpusrc, width, height);

    // ensure data is in physical ram
    GSPGPU_FlushDataCache(gpusrc, width * height * 4);

    // Load the texture and bind it to the first texture unit
    C3D_TexInit(&spritesheet_tex, width, height, GPU_RGBA8);

    // Convert image to 3DS tiled texture format
    u32 buffDimension = GX_BUFFER_DIM(width, height);
    C3D_SafeDisplayTransfer((u32*) gpusrc, buffDimension,
                            (u32*) spritesheet_tex.data,
                            buffDimension, TEXTURE_TRANSFER_FLAGS);
    gspWaitForPPF();

    C3D_TexSetFilter(&spritesheet_tex, GPU_LINEAR, GPU_NEAREST);

    imageDimension = {0.0f, 1.0f, 0.0f, 1.0f};
    this->width = 32;
    this->height = 32;

    free(image);
    linearFree(gpusrc);
}