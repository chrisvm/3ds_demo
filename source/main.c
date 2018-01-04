#include <3ds.h>
#include <stdio.h>


int main()
{
	// Initializations
	srvInit();        // services
	aptInit();        // applets
	hidInit();        // input
	gfxInitDefault(); // graphics
	gfxSet3D(false);  // stereoscopy (true: enabled / false: disabled)

	u32 kDown;        // keys down

	consoleInit(GFX_TOP, NULL);

	// get bottom screen framebuffers
	u16 fb_width, fb_height;
	u8 *fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &fb_width, &fb_height);

	printf("Screen Width: %u\n", fb_width);
	printf("Screen Height: %u\n", fb_height);
	printf("\x1b[21;16HPress Start to exit.");

	touchPosition touch;
	circlePosition circlePos;

	// Main loop
	while (aptMainLoop()) {
		// Read which buttons are currently pressed or not
		hidScanInput();

		// print touchpad
		hidTouchRead(&touch);
		printf("\x1b[8;0HX:%03d, Y:%03d", touch.px, touch.py);

		// print analog
		hidCircleRead(&circlePos);
		printf("\x1b[9;0HX:%03d, Y:%03d", circlePos.dx, circlePos.dy);

		// If START button is pressed, break loop and quit
		kDown = hidKeysDown();
		if (kDown & KEY_START) break;

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		// Wait for next frame
		gspWaitForVBlank();
	}

	// Exit
	gfxExit();
	hidExit();
	aptExit();
	srvExit();

	// Return to hbmenu
	return 0;
}
