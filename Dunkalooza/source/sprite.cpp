#include "sprite.h"

C3D_Mtx* Sprite::CalculateModelMatrix()
{
	Mtx_Identity(&model);
	Mtx_Translate(&model, this->x, this->y, 0, true);
	Mtx_RotateZ(&model, rotation, true);
	float x_trans = -this->origin_x * this->width;
	float y_trans = -this->origin_y * this->height;
	Mtx_Translate(&model, x_trans, y_trans, 0, true);
	return &model;
}

void Sprite::WriteToVBO(VBOEntry* vbo, int startingIndex)
{
	float left = imageDimension.left;
	float right = imageDimension.right;
	float top = imageDimension.top;
	float bottom = imageDimension.bottom;

	vbo[startingIndex + 0] = (VBOEntry){ 0, 0, 0.5f, left, top };
	vbo[startingIndex + 1] = (VBOEntry){ 0, height, 0.5f, left, bottom };
	vbo[startingIndex + 2] = (VBOEntry){ width, 0, 0.5f, right, top };

	vbo[startingIndex + 3] = (VBOEntry){ width, 0, 0.5f, right, top };
	vbo[startingIndex + 4] = (VBOEntry){ 0, height, 0.5f, left, bottom };
	vbo[startingIndex + 5] = (VBOEntry){ width, height, 0.5f, right, bottom };
}

void Sprite::BindToTextureUnit(int unitIndex) 
{
	C3D_TexBind(unitIndex, &spritesheet_tex);
}

void Sprite::LoadImage(const unsigned char* in, size_t size)
{
	unsigned char* image;
    unsigned width, height;

    lodepng_decode32(&image, &width, &height, in, size);

    u8 *gpusrc = (u8*) linearAlloc(width * height * 4);
    ShiftEndianess(image, gpusrc, width, height);

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

    free(image);
    linearFree(gpusrc);
}

void Sprite::ShiftEndianess(u8* src, u8* dst, unsigned width, unsigned height)
{
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

void Sprite::Draw(SceneContext* scene)
{
	// Update the uniforms
	C3D_Mtx *model = CalculateModelMatrix();
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, scene->uLoc_model, model);
	C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, scene->uLoc_projection, &scene->projection);
	BindToTextureUnit(0);
    C3D_DrawArrays(GPU_TRIANGLES, 0, 6);
}