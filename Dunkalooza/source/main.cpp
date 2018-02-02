#include <3ds.h>
#include <citro3d.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "scene_context.h"
#include "sprite.h"
#include "ship.h"

static void moveSprite(Sprite* sprite, u32 kDown);
static void printDebugInfo();
static SceneContext* sceneInit();

int main(int argc, char **argv) {
	// Initialize graphics
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	consoleInit(GFX_BOTTOM, NULL);

	// Initialize the render target
	C3D_RenderTarget* target = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8,
                                                      GPU_RB_DEPTH24_STENCIL8);
	C3D_RenderTargetSetClear(target, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
	C3D_RenderTargetSetOutput(target, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

	// Initialize the scene
	SceneContext *scene = sceneInit();

    // create sprites
    Ship *ship = new Ship();
	ship->origin_x = ship->origin_y = 0.5f;
	ship->ang_vel = 0.04;
	ship->Load();
	ship->WriteToVBO(scene->vbo, 0);

	// place on center
	ship->x = SCREEN_WIDTH / 2 - (ship->width / 2);
	ship->y = SCREEN_HEIGHT / 2 - (ship->height / 2);
	ship->dx = ship->dy = 0;

	// Main loop
	while (aptMainLoop()) {

		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) {
			break; // break in order to return to hbmenu
        }

		u32 kHeld = hidKeysHeld();
		moveSprite(ship, kHeld);
        printDebugInfo();

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_FrameDrawOn(target);
			ship->Draw(scene);
		C3D_FrameEnd(0);
	}

	// Deinitialize the scene
	delete scene;
	delete ship;

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}

static void moveSprite(Sprite* sprite, u32 kDown)
{
	sprite->x += sprite->dx;
	sprite->y += sprite->dy;

	if ((kDown & KEY_LEFT) != 0) {
		sprite->rotation -= sprite->ang_vel;
	}

	if ((kDown & KEY_RIGHT) != 0) {
		sprite->rotation += sprite->ang_vel;
	}

	if ((kDown & KEY_UP) != 0) {
		sprite->x += cos(sprite->rotation - M_PI_2) * 5;
		sprite->y += sin(sprite->rotation - M_PI_2) * 5;
	}
}

static void printDebugInfo()
{
    printf("\x1b[3;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime() * 6.0f);
    printf("\x1b[4;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime() * 6.0f);
    printf("\x1b[5;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage() * 100.0f);
}

static SceneContext* sceneInit()
{
	SceneContext* scene = new SceneContext();
	scene->InitShader();

	// Allocate VBO
    scene->vbo = (VBOEntry*) linearAlloc(sizeof(VBOEntry) * 6);

    // Configure buffers
	C3D_BufInfo* bufInfo = C3D_GetBufInfo();
	BufInfo_Init(bufInfo);
	BufInfo_Add(bufInfo, scene->vbo, sizeof(VBOEntry), 2, 0x10);

	// Compute the projection matrix
	// Note: we're setting top to 240 here so origin is at top left.
	Mtx_OrthoTilt(&scene->projection, 0.0, 400.0, 240.0, 0.0, 0.0, 1.0, true);

	return scene;
}
