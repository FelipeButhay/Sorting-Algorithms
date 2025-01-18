#pragma once
#include <ctime>
#include <utility>
#include <random>
#include <iostream>
#include "Cell.h"

void Shuffle(std::vector<int>& arr);

bool isSorted(const std::vector<int>& arr);

// SERIOUS ALGORITHMS

void BubbleSort(Cell& cell);
void BubbleSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void SelectionSort(Cell& cell);
void SelectionSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void DoubleSelectionSort(Cell& cell);
void DoubleSelectionSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void InsertionSort(Cell& cell);
void InsertionSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void BinaryInsertionSort(Cell& cell);
void BinaryInsertionSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

// ESOTERICS

void BogoSort(Cell& cell);
void BogoSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void StalinSort(Cell& cell);
void StalinSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void SleepSort(Cell& cell);
void SleepSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void MiracleSort(Cell& cell);
void MiracleSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void SnowFlakeSort(Cell& cell);
void SnowFlakeSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void BozoSort(Cell& cell);
void BozoSort(std::vector<int>& arr, std::atomic<bool>& isSorting);