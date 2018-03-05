#include <3ds.h>
#include <citro3d.h>
#include <physics.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"
#include "scene_context.h"
#include "sprite.h"
#include "ship.h"
#include "bullet_manager.h"
#include "bullet.h"

static void moveShip(Ship* sprite, u32 kDown, float deltaTime);
static void printDebugInfo();

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
	SceneContext *scene = new SceneContext();

	// create sprites
	Ship *ship = new Ship();
	ship->origin.x = ship->origin.y = 0.5f;
	ship->ang_vel = 3;
	ship->Load();
	ship->WriteToVBO(scene->vbo);

	// place on center
	ship->pos.x = SCREEN_WIDTH / 2 - (ship->width / 2);
	ship->pos.y = SCREEN_HEIGHT / 2 - (ship->height / 2);
	ship->vel.x = ship->vel.y = 0;
	ship->CreatePhysicsBody(&scene->phy_scene);

	// create bullet manager
	BulletManager *b_manager = new BulletManager();
	b_manager->WriteToVBO(scene->vbo);
	b_manager->SetScreenSize(vec2f(SCREEN_WIDTH, SCREEN_HEIGHT));

	// Main loop
	while (aptMainLoop()) {

		hidScanInput();
		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) {
			break; // break in order to return to hbmenu
		}

		u32 kHeld = hidKeysHeld();
		moveShip(ship, kHeld, DELTA_TIME);

		b_manager->Update(DELTA_TIME);
		if ((kDown & KEY_A) != 0) {
			b_manager->CreateBullet(ship->pos.x, ship->pos.y, ship->rotation);
		}

		printDebugInfo();

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C3D_FrameDrawOn(target);
			ship->Draw(scene);
			b_manager->Draw(scene);
		C3D_FrameEnd(0);
	}

	// Deinitialize the scene
	delete scene;
	delete b_manager;
	delete ship;

	// Deinitialize graphics
	C3D_Fini();
	gfxExit();
	return 0;
}

static void moveShip(Ship* sprite, u32 kDown, float deltaTime)
{
	if ((kDown & KEY_LEFT) != 0) {
		sprite->rotation -= sprite->ang_vel * deltaTime;
	}

	if ((kDown & KEY_RIGHT) != 0) {
		sprite->rotation += sprite->ang_vel * deltaTime;
	}

	if ((kDown & KEY_B) != 0) {
		sprite->MoveToFacing(80 * DELTA_TIME);
	}
}

static void printDebugInfo()
{
	printf("\x1b[3;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime() * 6.0f);
	printf("\x1b[4;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime() * 6.0f);
	printf("\x1b[5;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage() * 100.0f);
}
