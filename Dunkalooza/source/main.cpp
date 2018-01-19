#include <3ds.h>
#include <citro3d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lodepng.h"
#include "types.h"
#include "scene_context.h"
#include "drawing.h"

// These headers are generated by the build process
#include "kirball_png.h"

static ImageDimension image = {0.0f, 1.0f, 0.0f, 1.0f};

static void printDebugInfo(uint numSprites) {
    printf("\x1b[1;0HSprites: %zu\x1b[K", numSprites);
    printf("\x1b[3;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime() * 6.0f);
    printf("\x1b[4;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime() * 6.0f);
    printf("\x1b[5;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage() * 100.0f);
}

static SceneContext* sceneInit() {
	SceneContext* scene = new SceneContext();
	scene->InitShader();

	// Get the location of the uniforms
	scene->uLoc_projection = shaderInstanceGetUniformLocation(scene->program.vertexShader, "projection");

	// Allocate VBO
    scene->vbo = (VBOEntry*) linearAlloc(sizeof(VBOEntry) * 6);

	// Configure attributes for use with the vertex shader
	// Attribute format and element count are ignored in immediate mode
	C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
	AttrInfo_Init(attrInfo);
	AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // v0=position
	AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // v2=texcoord

	// Compute the projection matrix
	// Note: we're setting top to 240 here so origin is at top left.
	Mtx_OrthoTilt(&scene->projection, 0.0, 400.0, 240.0, 0.0, 0.0, 1.0, true);

	// Configure buffers
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, scene->vbo, sizeof(VBOEntry), 2, 0x10);

	unsigned char* image;
	unsigned width, height;

	lodepng_decode32(&image, &width, &height, kirball_png, kirball_png_size);

	u8 *gpusrc = (u8*) linearAlloc(width * height * 4);

    shiftEndianess(image, gpusrc, width, height);

	// ensure data is in physical ram
	GSPGPU_FlushDataCache(gpusrc, width * height * 4);

	// Load the texture and bind it to the first texture unit
	C3D_TexInit(&scene->spritesheet_tex, width, height, GPU_RGBA8);

	// Convert image to 3DS tiled texture format
    u32 buffDimension = GX_BUFFER_DIM(width, height);
	C3D_SafeDisplayTransfer((u32*) gpusrc, buffDimension, (u32*) scene->spritesheet_tex.data, buffDimension, TEXTURE_TRANSFER_FLAGS);
	gspWaitForPPF();

	C3D_TexSetFilter(&scene->spritesheet_tex, GPU_LINEAR, GPU_NEAREST);
	C3D_TexBind(0, &scene->spritesheet_tex);

	free(image);
	linearFree(gpusrc);

	// Configure the first fragment shading substage to just pass through the texture color
	// See https://www.opengl.org/sdk/docs/man2/xhtml/glTexEnv.xml for more insight
	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, 0, 0);
	C3D_TexEnvOp(env, C3D_Both, 0, 0, 0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

	// Configure depth test to overwrite pixels with the same depth (needed to draw overlapping sprites)
	C3D_DepthTest(true, GPU_GEQUAL, GPU_WRITE_ALL);

	return scene;
}

int main(int argc, char **argv) {
	// Initialize graphics
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	consoleInit(GFX_BOTTOM, NULL);

	// Initialize the render target
	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	// Initialize the scene
	SceneContext *scene = sceneInit();

    // create sprites
    Sprite *sprites = new Sprite();
    uint numSprites = 1;

    srand(time(NULL));

	for (uint i = 0; i < numSprites; i++) {
		//random place and speed
		sprites[i].x = rand() % (SCREEN_WIDTH - 32);
		sprites[i].y = rand() % (SCREEN_HEIGHT - 32);
		sprites[i].dx = rand()*4.0f/RAND_MAX - 2.0f;
		sprites[i].dy = rand()*4.0f/RAND_MAX - 2.0f;
		sprites[i].image = rand() & 3;
	}

	// Main loop
	while (aptMainLoop()) {

		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) {
			break; // break in order to return to hbmenu
        }

		moveSprites(sprites, numSprites);

        printDebugInfo(numSprites);

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_FrameDrawOn(target);
			drawSprites(scene, sprites, numSprites, image);
		C3D_FrameEnd(0);
	}

	// Deinitialize the scene
	delete scene;

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}
