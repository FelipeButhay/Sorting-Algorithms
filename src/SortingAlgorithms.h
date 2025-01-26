#pragma once
#include <ctime>
#include <utility>
#include <random>
#include <iostream>
#include "raylib.h"
#include "Cell.h"

void ShuffleVisual(Cell& cell);
void Shuffle(Cell& cell);

void ShufflePro(std::vector<int>& arr);

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

void ShakerSort(Cell& cell);
void ShakerSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void QuickSort(Cell& cell);
void QuickSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void MergeSort(Cell& cell);
void MergeSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void HeapSort(Cell& cell);
void HeapSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void CombSort(Cell& cell);
void CombSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void ShellSort(Cell& cell);
void ShellSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void BucketSort(Cell& cell);
void BucketSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void CountingSort(Cell& cell);
void CountingSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void LSDRadixSort(Cell& cell);
void LSDRadixSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void MSDRadixSort(Cell& cell);
void MSDRadixSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void PancakeSort(Cell& cell);
void PancakeSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void CycleSort(Cell& cell);
void CycleSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void ExchangeSort(Cell& cell);
void ExchangeSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void PatienceSort(Cell& cell);
void PatienceSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void OddEvenSort(Cell& cell);
void OddEvenSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

// ESOTERICS

void BogoSort(Cell& cell);
void BogoSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void StalinSort(Cell& cell);
void StalinSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void SleepSort(Cell& cell);
void SleepSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void MiracleSort(Cell& cell);
void MiracleSort(std::vector<int>& arr, std::atomic<bool>& isSorting);

void BozoSort(Cell& cell);
void BozoSort(std::vector<int>& arr, std::atomic<bool>& isSorting);