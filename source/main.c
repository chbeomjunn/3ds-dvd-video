#include <citro2d.h>
#include <3ds.h>

#include "dvd.h"

int main() {
	// Targets for left and right eye respectively
	C3D_RenderTarget* left;
	C3D_RenderTarget* right;
	
	C2D_SpriteSheet sheet;
	C2D_Image dvd_image;

	int keysD;
	int keysH;
	float slider;

	// Per-eye offsets
	int offsetUpper = 0;
	int offsetLower = 0;

	// Initialize libraries
	romfsInit();
	gfxInitDefault();
	gfxSet3D(true); // Activate stereoscopic 3D
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	consoleInit(GFX_BOTTOM, NULL);

	printf("\x1b[1;1H Stereoscopic 3D with citro2d\n");

	// Create targets for both eyes on the top screen
	left = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	right = C2D_CreateScreenTarget(GFX_TOP, GFX_RIGHT);

	// Load the lenny dvd_image
	sheet = C2D_SpriteSheetLoad("romfs:/gfx/dvd.t3x");	
	dvd_image = C2D_SpriteSheetGetImage(sheet, 0);

    // set positions
    float dvd_x = 100.0f;
    float dvd_y = 100.0f;
    float dvd_z = 100.0f;
    float dvd_velocity_x = 0.0f;
    float dvd_velocity_y = 0.0f;
    float dvd_velocity_z = 0.0f;

    C2D_ImageTint tint = {100, 100, 100, 100};

	while (aptMainLoop()) {
		// Handle user input
		hidScanInput();
		keysD = hidKeysDown();
		keysH = hidKeysHeld();

		if (keysD & KEY_START) break;

		slider = osGet3DSliderState();

		// Print "useful" information
		printf("\x1b[3;1H %.2f | %i | %i | %f | %f | %f |         \n", slider, offsetUpper, offsetLower, dvd_x, dvd_y, dvd_z);

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		// Render the left eye's view
		{
			C2D_TargetClear(left, C2D_Color32(0xff, 0xff, 0xff, 0xff));
			C2D_SceneBegin(left);

			C2D_DrawImageAt(dvd_image, dvd_x + offsetUpper * slider, dvd_y, dvd_z, &tint, 1.0f, 1.0f);		
			C2D_DrawImageAt(dvd_image, dvd_x + offsetLower * slider, dvd_y + 100.0f, dvd_z, &tint, 1.0f, 1.0f);
		}

		// Render the right eye's view 
		{
			C2D_TargetClear(right, C2D_Color32(0xff, 0xff, 0xff, 0xff));
			C2D_SceneBegin(right);

			C2D_DrawImageAt(dvd_image, dvd_x + offsetUpper * slider, dvd_y, dvd_z, &tint, 1.0f, 1.0f);
			C2D_DrawImageAt(dvd_image, dvd_x + offsetLower * slider, dvd_y + 100.0f, dvd_z, &tint, 1.0f, 1.0f);
		}
		C3D_FrameEnd(0);
        dvd_x += dvd_velocity_x;
        dvd_y += dvd_velocity_y;
        dvd_z += dvd_velocity_z;
	}

	C2D_SpriteSheetFree(sheet);

	// De-initialize libraries
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
}
