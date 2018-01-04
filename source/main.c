#include <3ds.h>
#include <stdio.h>

void print_inputs(u32 kDown, u32 kHold)
{
    // print touchpad
    touchPosition touch;
    hidTouchRead(&touch);
    printf("\x1b[8;0HX:%03d, Y:%03d", touch.px, touch.py);

    // print analog
    circlePosition circlePos;
    hidCircleRead(&circlePos);
    printf("\x1b[9;0HX:%03d, Y:%03d ", circlePos.dx, circlePos.dy);

    u32 downAndHold = kDown | kHold;

    if (downAndHold & KEY_A) {
        printf("\x1b[10;0H\x1b[32mA Button");
    } else {
        printf("\x1b[10;0H\x1b[37mA Button");
    }

    if (downAndHold & KEY_B) {
        printf("\x1b[11;0H\x1b[32mB Button");
    } else {
        printf("\x1b[11;0H\x1b[37mB Button");
    }

    if (downAndHold & KEY_X) {
        printf("\x1b[12;0H\x1b[32mX Button");
    } else {
        printf("\x1b[12;0H\x1b[37mX Button");
    }

    if (downAndHold & KEY_Y) {
        printf("\x1b[13;0H\x1b[32mY Button");
    } else {
        printf("\x1b[13;0H\x1b[37mY Button");
    }

    printf("\x1b[37m");
}

int main()
{
	// Initialization
	srvInit();        // services
	aptInit();        // applets
	hidInit();        // input
	gfxInitDefault(); // graphics
	gfxSet3D(false);  // stereoscopy (true: enabled / false: disabled)

	u32 kDown;        // keys down
    u32 kHold;

	consoleInit(GFX_TOP, NULL);

	// get bottom screen framebuffers
	u16 fb_width, fb_height;
	u8 *fb = gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, &fb_width, &fb_height);

	printf("Screen Width: %u\n", fb_width);
	printf("Screen Height: %u\n", fb_height);
	printf("\x1b[21;16HPress Start to exit.");

	// Main loop
	while (aptMainLoop()) {
		// Read which buttons are currently pressed or not
		hidScanInput();

		// If START button is pressed, break loop and quit
		kDown = hidKeysDown();
        kHold = hidKeysHeld();

		if (kDown & KEY_START) break;

        print_inputs(kDown, kHold);

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
