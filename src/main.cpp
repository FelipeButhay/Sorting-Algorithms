#include "Display.h"

int main(void) {
	Display display;

	while (!WindowShouldClose()) {
		display.loop();

		BeginDrawing();
			display.draw();
		EndDrawing();
	}
	
	return 0;
}