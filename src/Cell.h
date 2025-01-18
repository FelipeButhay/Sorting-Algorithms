#pragma once
#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>

typedef struct Cell {
	short x;
	short y;
	short width;
	short height;
	short id;

	int interval;
	std::atomic<int> iter1;
	std::atomic<int> iter2;
	std::atomic<int> iter3;
	std::atomic<int> iter4;

	bool visual;
	bool alive;

	void (*algorithmFunc)(std::vector<int>&, std::atomic<bool>&);
	void (*algorithmFuncVisual)(Cell&);

	std::atomic<bool> isSorting;

	std::chrono::steady_clock::time_point executionTimeStart;
	std::chrono::steady_clock::time_point executionTimeEnd;
	std::chrono::duration<double, std::milli> executionTime;

	std::vector<int> array;

	std::thread thread;
	std::mutex mutex;
};
