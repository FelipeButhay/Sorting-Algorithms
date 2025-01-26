#include "Display.h"

int main(void) {
	Display display;

	while (true) {
		display.loop();

		if (WindowShouldClose()) break;

		BeginDrawing();
			display.draw();
		EndDrawing();
	}
	
	return 0;
}