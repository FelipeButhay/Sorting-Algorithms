#include "Display.h"

Display::Display() {
	this->resolution = 0;
	this->sx = difResolutions[resolution%8][0];
	this->sy = difResolutions[resolution%8][1];
	this->u = sy*0.05f;

	InitWindow(sx, sy, "Sorting Algorithms Visualizer");
	SetTargetFPS(60);

	this->stage = 0;

	this->usingAlgorithmsN = 0;

	this->algorithmsInScreenMD = (sy - (5+1)*u)/(lineSizeU*u);
	this->topAlgorithmMD = 0;
	this->topAlgorithmBoundsMD[0] = 0;
	this->topAlgorithmBoundsMD[1] = algorithmsN <= algorithmsInScreenMD ? 0 : algorithmsN - algorithmsInScreenMD;

	this->barSize = algorithmsInScreenMD*lineSizeU*u;
	this->barPercentageSize = algorithmsInScreenMD/((float)algorithmsN);
	if (barPercentageSize >= 1) this->barPercentageSize = 1;

	this->barPercentagePos = topAlgorithmMD/((float)algorithmsN);

	this->sampleSize = 0;
	this->visualizerTime = 0;
}

void Display::loop() {
	switch (stage) {
		case 0: menuLoop();       break;
		case 1: settingsLoop();   break;
		case 2: visualizerLoop(); break;
	}
}

void Display::draw() {
	switch (stage) {
		case 0: menuDraw();       break;
		case 1: settingsDraw();   break;
		case 2: visualizerDraw(); break;
	}
}

void Display::measureAlgorithmTime(Cell* cell) {
	cell->isSorting = true;
	cell->executionTimeStart = std::chrono::high_resolution_clock::now();

	if (cell->visual) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		ShuffleVisual(*cell);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		cell->algorithmFuncVisual(*cell);
	} else {
		Shuffle(*cell);
		cell->algorithmFunc(cell->array, cell->isSorting);
	}

	std::cout << "El array se ordeno? " << isSorted(cell->array) << " ID: " << cell->id << '\n';
	cell->executionTimeEnd = std::chrono::high_resolution_clock::now();

	cell->iter1 = -1;
	cell->iter2 = -1;
	cell->iter3 = -1;
	cell->iter4 = -1;
	cell->iter5 = -1;

	cell->executionTime = cell->executionTimeEnd - cell->executionTimeStart;
	cell->isSorting = false;
}
