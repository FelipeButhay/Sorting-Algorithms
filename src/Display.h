#pragma once
#include <iostream>
#include "raylib.h"
#include "Tools.h"
#include "SortingAlgorithms.h"

#define BACKGROUND CLITERAL(Color){  50,  50,  50, 255 }
#define LIGHTBLACK CLITERAL(Color){  20,  20,  20, 255 }
#define DARKWHITE  CLITERAL(Color){ 170, 170, 170, 255 }
#define DARKRED    CLITERAL(Color){ 135,   9,   9, 255 }

#define algorithmsN 25
#define lineSizeU 1.4

#define samplesN 20
#define timesN 14

#define M 1000

using chronoMoment = std::chrono::steady_clock::time_point;
using chronoDuration = std::chrono::duration<double, std::milli>;

class Display {
	private:
	short sx, sy, u;
	unsigned short resolution;
	short difResolutions[8][2] = { {  900,  500 }, { 1280,  720 }, { 1366,  768 }, { 1536,  864 },
								   { 1600,  900 }, { 1920, 1080 }, { 2560, 1440 }, { 3840, 2160 } };

	// 0 menu, 1 settings, 2 visualizer
	unsigned short stage;

	// lista de los nombres de los algoritmos
	std::string algorithms[algorithmsN] = {
		"Bubble",
		"Shaker",
		"Selection",
		"Double Selection",
		"Insertion",
		"Binary Insertion",

		"Quick",
		"Merge",
		"Heap",
		"Comb",
		"Shell",
		"Bucket",
		"Counting",
		"LSD Radix",
		"MSD Radix",

		"Pancake",
		"Cycle",
		"Exchange",
		"Patience",
		"Odd Even",

		"*Bogo",
		"*Stalin",
		"*Sleep",
		"*Miracle",
		"*Bozo"
	};

	// lista de punteros a las funciones de los algoritmos
	void (*algorithmFuncPtr[algorithmsN])(std::vector<int>&, std::atomic<bool>&) = {
		BubbleSort,
		ShakerSort,
		SelectionSort,
		DoubleSelectionSort,
		InsertionSort,
		BinaryInsertionSort,

		QuickSort,
		MergeSort,
		HeapSort,
		CombSort,
		ShellSort,
		BucketSort,
		CountingSort,
		LSDRadixSort,
		MSDRadixSort,

		PancakeSort,
		CycleSort,
		ExchangeSort,
		PatienceSort,
		OddEvenSort,

		BogoSort,
		StalinSort,
		SleepSort,
		MiracleSort,
		BozoSort
	};
	void (*algorithmFuncVisualPtr[algorithmsN])(Cell&) = {
		BubbleSort,
		ShakerSort,
		SelectionSort,
		DoubleSelectionSort,
		InsertionSort,
		BinaryInsertionSort,

		QuickSort,
		MergeSort,
		HeapSort,
		CombSort,
		ShellSort,
		BucketSort,
		CountingSort,
		LSDRadixSort,
		MSDRadixSort,

		PancakeSort,
		CycleSort,
		ExchangeSort,
		PatienceSort,
		OddEvenSort,

		BogoSort,
		StalinSort,
		SleepSort,
		MiracleSort,
		BozoSort
	};

	// -1 vacio, >= 0 el indx de Algorithms, le agrego un espacio mas para el codigo de la caida
	short usingAlgorithmsID[5] = { -1 };
	//cantidad de algoritmos en uso
	short usingAlgorithmsN;

	// >> MENU DRAW RELATED VARIABLES <<

	// indx del primer algoritmo en la lista + scroll
	short topAlgorithmMD;
	// limites de la rueda
	short topAlgorithmBoundsMD[2];

	// la cantidad de algoritmos que podes ver en el menu en simultaneo
	short algorithmsInScreenMD;

	// barra de scroll
	float barPercentageSize;
	float barPercentagePos;

	short barSize;
	short clickBarY;
	short topAlgorithmMDSave;

	bool barMoving;

	// >> VISUALIZER RELATED VARIABLES <<
	short sampleSize;
	int possibleSamples[samplesN] = { 5, 10, 20, 50, 100, 200, 500,
									  1*M, 2*M, 5*M, 10*M, 20*M, 50*M, 100*M, 200*M, 500*M,
									  1*M*M, 2*M*M, 5*M*M, 10*M*M };
	std::string possibleSamplesStr[samplesN] = { "5", "10", "20", "50", "100", "200", "500", "1.000", "2.000",
												 "5.000", "10.000", "20.000", "50.000", "100.000", "200.000",
												 "500.000", "1.000.000", "2.000.000", "5.000.000", "10.000.000" };

	short visualizerTime;
	int possibleTimes[timesN] = { 0, 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000 };
	std::string possibleTimesStr[timesN] = { "0", "1", "2", "5", "10", "20", "50", "100", "200", "500", "1", "2", "5", "10" };

	Cell cells[4];

	void measureAlgorithmTime(Cell* cell);

	void menuLoop();
	void menuDraw();

	void settingsLoop();
	void settingsDraw();

	void visualizerLoop();
	void visualizerDraw();

	void visualizeCell(Cell* cell);

	public:
	Display();
	void loop();
	void draw();
};