#include "SortingAlgorithms.h"
#include <ctime>
#include <utility>
#include <random>
#include <iostream>

// SHUFFLE
void ShuffleVisual(Cell& cell) {
    // std::atomic<int>* i = &cell.iter1;
    // std::atomic<int>* j = &cell.iter2;
    int n = cell.array.size();

    for (int i = 0; i < n; i++) {
        int j = cell.rNumbers[i];
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            std::swap(cell.array[i], cell.array[j]);
        }
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval*3));
    }
}

void Shuffle(Cell& cell) {
    int n = cell.array.size();

    for (int i = 0; i < n; i++) {
        int j = cell.rNumbers[i];
        std::swap(cell.array[i], cell.array[j]);

        if (!cell.isSorting) return;
    } 
}

void ShufflePro(std::vector<int>& arr) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, arr.size() - 1);

    for (int i = 0; i<arr.size(); i++) {
        int j = dis(gen);
        std::swap(arr[i], arr[j]);
    }
}

// IS SORTED
bool isSorted(const std::vector<int>& arr) {
    for (int i = 0; i<arr.size()-1; i++) {
        if (arr[i] > arr[i+1]) {
            return false;
        }
    }
    return true;
}

// >> SERIOUS ALGORITHMS <<

// BUBBLE SORT
void BubbleSort(Cell& cell) {
    std::atomic<int>* i = &cell.iter1;
    std::atomic<int>* j = &cell.iter2;

    int n = cell.array.size();
    while (n > 1) {
        int lastSwap = 0;

        for (*i = 0, *j = 1; *i < n - 1; (*i)++, (*j)++) {
            if (cell.array[*i] > cell.array[*i + 1]) {
                std::lock_guard<std::mutex> lock(cell.mutex);
                std::swap(cell.array[*i], cell.array[*j]);
                lastSwap = *i + 1;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }

        n = lastSwap;
    }
}

void BubbleSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int n = arr.size();
    while (n > 1) {
        int lastSwap = 0;

        int i;
        for (i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                lastSwap = i + 1;
            }
            if (!isSorting) return;
        }

        n = lastSwap;
    }
}

// SELECTION SORT
void SelectionSort(Cell& cell) {
    int arraySize = cell.array.size();
    for (cell.iter1 = 0; cell.iter1<arraySize-1; cell.iter1++) {
        int minflag = cell.iter1;
        for (cell.iter2 = cell.iter1 + 1; cell.iter2<arraySize; cell.iter2++) {
            if (cell.array[minflag] > cell.array[cell.iter2]) {
                minflag = cell.iter2;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }

        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            std::swap(cell.array[minflag], cell.array[cell.iter1]);
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void SelectionSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int arraySize = arr.size();
    for (int i = 0; i<arraySize-1; i++) {
        int minflag = i;
        for (int j = i + 1; j<arraySize; j++) {
            if (arr[minflag] > arr[j]) {
                minflag = j;
            }
        }

        std::swap(arr[minflag], arr[i]);
        if (!isSorting) return;
    }
}

// DOUBLE SELECTION SORT
void DoubleSelectionSort(Cell& cell) {
    for (int i = 0; i < cell.array.size(); i++) {
        cell.iter1 = i;
        cell.iter2 = cell.iter1 + 1;

        int minflag = cell.iter1;
        for (; cell.iter2 <= cell.array.size() - i - 1; cell.iter2++) {
            if (cell.array[minflag] > cell.array[cell.iter2]) {
                minflag = (int)cell.iter2;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            std::swap(cell.array[cell.iter1], cell.array[minflag]);
        }

        cell.iter1 = -1;
        cell.iter2 = -1;

        cell.iter3 = cell.array.size() - i - 1;
        cell.iter4 = cell.iter3 - 1;

        int maxflag = cell.iter3;
        for (; cell.iter4 >= i; cell.iter4--) {
            if (cell.array[maxflag] < cell.array[cell.iter4]) {
                maxflag = (int)cell.iter4;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            if (maxflag == i) maxflag = minflag;
            std::swap(cell.array[cell.iter3], cell.array[maxflag]);
        }

        cell.iter3 = -1;
        cell.iter4 = -1;
    }
}

void DoubleSelectionSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    for (int i = 0; i < arr.size(); i++) {
        int minflag = i;
        int maxflag = arr.size() - i - 1;

        for (int j = minflag + 1; j <= arr.size() - i - 1; j++) {
            if (arr[minflag] > arr[j]) {
                minflag = j;
            }
        }
        for (int j = maxflag - 1; j >= i; j--) {
            if (arr[maxflag] < arr[j]) {
                maxflag = j;
            }
        }

        std::swap(arr[minflag], arr[i]);
        if (maxflag == i) maxflag = minflag;
        std::swap(arr[maxflag], arr[arr.size() - i - 1]);

        if (!isSorting) return;
    }
}

// INSERTION SORT
void InsertionSort(Cell& cell) {
    for (cell.iter1 = 1; cell.iter1 < cell.array.size(); cell.iter1++) {
        for (cell.iter2 = (int)cell.iter1; cell.iter2 > 0; cell.iter2--) {

            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));

            if (cell.array[cell.iter2] < cell.array[cell.iter2-1]) {
                std::lock_guard<std::mutex> lock(cell.mutex);
                std::swap(cell.array[cell.iter2], cell.array[cell.iter2-1]);
            } else {
                break;
            }
        }
    }
}

void InsertionSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    for (int i = 1; i<arr.size(); i++) {
        for (int j = i; j > 0; j--) {
            if (arr[j] < arr[j-1]) {
                std::swap(arr[j], arr[j-1]);
            } else {
                break;
            }
        }
    }
}

// BINARY INSERTION SORT
void BinaryInsertionSort(Cell& cell) {
    for (cell.iter1 = 1; cell.iter1 < cell.array.size(); cell.iter1++) {
        int key = cell.array[cell.iter1];
        int left = 0;
        int right = cell.iter1 - 1;

        while (left <= right) {
            if (!cell.isSorting) return;

            cell.iter2 = left + (right - left) / 2;
            if (key < cell.array[cell.iter2]) {
                right = cell.iter2 - 1;
            } else {
                left = cell.iter2 + 1;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval*5));
        }

        {
            std::lock_guard<std::mutex> lock(cell.mutex);

            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
            cell.array.erase(cell.array.begin() + cell.iter1);

            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
            cell.array.insert(cell.array.begin() + left, key);

        }
    }
}

void BinaryInsertionSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    for (int i = 1; i < (int)arr.size(); i++) {
        int key = arr[i];
        int left = 0;
        int right = i - 1;

        while (left <= right) {
            if (!isSorting) return;

            int mid = left + (right - left) / 2;
            if (key < arr[mid]) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        arr.erase(arr.begin() + i);
        arr.insert(arr.begin() + left, key);
    }
}

// SHAKER SORT
void ShakerSort(Cell& cell) {
    int rightMovingLastSwap = cell.array.size();
    int leftMovingLastSwap = 0;
    while (rightMovingLastSwap > leftMovingLastSwap) {

        int tempRMLS = 0;
        for (cell.iter1 = leftMovingLastSwap; cell.iter1 < rightMovingLastSwap - 1; cell.iter1++) {

            if (cell.array[cell.iter1] > cell.array[cell.iter1 + 1]) {
                std::lock_guard<std::mutex> lock(cell.mutex);
                std::swap(cell.array[cell.iter1], cell.array[cell.iter1 + 1]);
                tempRMLS = cell.iter1 + 1;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }
        rightMovingLastSwap = tempRMLS;

        int tempLMLS = 0;
        for (cell.iter1 = rightMovingLastSwap; cell.iter1 > leftMovingLastSwap; cell.iter1--) {
            if (cell.array[cell.iter1] < cell.array[cell.iter1 - 1]) {
                std::lock_guard<std::mutex> lock(cell.mutex);
                std::swap(cell.array[cell.iter1], cell.array[cell.iter1 - 1]);
                tempLMLS = cell.iter1 - 1;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }
        leftMovingLastSwap = tempLMLS;
    }
}

void ShakerSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int rightMovingLastSwap = arr.size();
    int leftMovingLastSwap = 0;
    while (rightMovingLastSwap > leftMovingLastSwap) {

        int tempRMLS = 0;
        for (int i = leftMovingLastSwap; i < rightMovingLastSwap - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                std::swap(arr[i], arr[i + 1]);
                tempRMLS = i + 1;
            }
            if (!isSorting) return;
        }
        rightMovingLastSwap = tempRMLS;

        int tempLMLS = 0;
        for (int j = rightMovingLastSwap; j > leftMovingLastSwap + 1; j--) {
            if (arr[j] < arr[j - 1]) {
                std::swap(arr[j], arr[j - 1]);
                tempLMLS = j - 1;
            }
            if (!isSorting) return;
        }
        leftMovingLastSwap = tempLMLS;
    }
}

// QUICK SORT
int Partition(Cell& cell, const int indxF, const int indxL) {
    const int pivot = cell.array[indxL];
    std::atomic<int>* a = &cell.iter1;
    std::atomic<int>* b = &cell.iter2;

    *b = indxF - 1;

    for (*a = indxF; *a < indxL; (*a)++) {
        if (cell.array[*a] <= pivot) {
            std::lock_guard<std::mutex> lock(cell.mutex);
            (*b)++;
            std::swap(cell.array[*a], cell.array[*b]);
        }
        if (!cell.isSorting) return -69;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    {
        std::lock_guard<std::mutex> lock(cell.mutex);
        std::swap(cell.array[*b + 1], cell.array[indxL]);
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    return *b + 1;
}

void QuickSort(Cell& cell, const int indxF, const int indxL) {
    std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    if (indxF < indxL) {
        const int midIndx = Partition(cell, indxF, indxL);
        if (midIndx == -69) return;

        QuickSort(cell, indxF, midIndx - 1);
        QuickSort(cell, midIndx + 1, indxL);
    }
}

void QuickSort(Cell& cell) {
    const int midIndx = Partition(cell, 0, cell.array.size() - 1);

    QuickSort(cell, 0, midIndx - 1);
    QuickSort(cell, midIndx + 1, cell.array.size() - 1);
}


int Partition(std::vector<int>& arr, std::atomic<bool>& isSorting, const int indxF, const int indxL) {
    const int pivot = arr[indxL];
    int j = indxF - 1;

    for (int i = indxF; i < indxL; i++) {
        if (arr[i] <= pivot) {
            (j)++;
            std::swap(arr[i], arr[j]);
        }
        if (!isSorting) return -69;
    }

    std::swap(arr[j + 1], arr[indxL]);

    return j + 1;
}

void QuickSort(std::vector<int>& arr, std::atomic<bool>& isSorting, const int indxF, const int indxL) {
    if (indxF < indxL) {
        const int midIndx = Partition(arr, isSorting, indxF, indxL);
        if (midIndx == -69) return;

        QuickSort(arr, isSorting, indxF, midIndx - 1);
        QuickSort(arr, isSorting, midIndx + 1, indxL);
    }
}

void QuickSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    const int midIndx = Partition(arr, isSorting, 0, arr.size() - 1);

    QuickSort(arr, isSorting, 0, midIndx - 1);
    QuickSort(arr, isSorting, midIndx + 1, arr.size() - 1);
}

//MERGE SORT
void Merge(Cell& cell, int indxF, int indxM, int indxL) {
    if (!cell.isSorting) return;

    int sizeL = indxM - indxF + 1;
    int sizeR = indxL - indxM;

    std::vector<int> Left(sizeL), Right(sizeR);
    std::atomic<int>* it = &cell.iter2;
    for (int i = 0; i<sizeL; i++) {
        *it = indxF + i;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        Left[i] = cell.array[*it];
    }
    for (int i = 0; i<sizeR; i++) {
        *it = indxM + i + 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        Right[i] = cell.array[*it];
    }
    *it = -1;

    int i = 0, j = 0;
    std::atomic<int>* k = &cell.iter1;

    *k = indxF;

    while (i < sizeL && j < sizeR) {
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));

        if (Left[i] <= Right[j]) {
            std::lock_guard<std::mutex> lock(cell.mutex);
            cell.array[*k] = Left[i];
            i++;
        } else {
            std::lock_guard<std::mutex> lock(cell.mutex);
            cell.array[*k] = Right[j];
            j++;
        }

        (*k)++;
    }

    while (i < sizeL) {
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        std::lock_guard<std::mutex> lock(cell.mutex);
        cell.array[*k] = Left[i];
        i++;
        (*k)++;
    }

    while (j < sizeR) {
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        std::lock_guard<std::mutex> lock(cell.mutex);
        cell.array[*k] = Right[j];
        j++;
        (*k)++;
    }
}

void MergeSort(Cell& cell, int indxF, int indxL) {
    if (!cell.isSorting) return;
    if (indxF >= indxL) return;

    int indxM = indxF + (indxL - indxF)/2;
    MergeSort(cell, indxF, indxM);
    MergeSort(cell, indxM + 1, indxL);
    Merge(cell, indxF, indxM, indxL);
}

void MergeSort(Cell& cell) {
    MergeSort(cell, 0, cell.array.size() - 1);
}


void Merge(std::vector<int>& arr, std::atomic<bool>& isSorting, int indxF, int indxM, int indxL) {
    if (!isSorting) return;
    int sizeL = indxM - indxF + 1;
    int sizeR = indxL - indxM;

    std::vector<int> Left(sizeL), Right(sizeR);
    for (int i = 0; i<sizeL; i++) {
        Left[i] = arr[indxF + i];
    }
    for (int i = 0; i<sizeR; i++) {
        Right[i] = arr[indxM + i + 1];
    }

    int i = 0, j = 0, k = indxF;
    while (i < sizeL && j < sizeR) {
        if (!isSorting) return;

        if (Left[i] <= Right[j]) {
            arr[k] = Left[i];
            i++;
        } else {
            arr[k] = Right[j];
            j++;
        }

        k++;
    }

    while (i < sizeL) {
        if (!isSorting) return;
        arr[k] = Left[i];
        i++;
        k++;
    }

    while (j < sizeR) {
        if (!isSorting) return;
        arr[k] = Right[j];
        j++;
        k++;
    }
}

void MergeSort(std::vector<int>& arr, std::atomic<bool>& isSorting, int indxF, int indxL) {
    if (!isSorting) return;
    if (indxF >= indxL) return;

    int indxM = indxF + (indxL - indxF)/2;
    MergeSort(arr, isSorting, indxF, indxM);
    MergeSort(arr, isSorting, indxM + 1, indxL);
    Merge(arr, isSorting, indxF, indxM, indxL);
}

void MergeSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    MergeSort(arr, isSorting, 0, arr.size() - 1);
}

// HEAP SORT
void MaxHeapify(Cell& cell, const int i, const int n) {
    if (!cell.isSorting) return;
    std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));

    std::atomic<int>* father = &cell.iter2;
    std::atomic<int>* child1 = &cell.iter3;
    std::atomic<int>* child2 = &cell.iter4;

    *father = i;
    *child1 = 2*(*father) + 1;
    *child2 = 2*(*father) + 2;

    if (*child1 < n && cell.array[*child1] > cell.array[*father]) {
        *father = (*child1)*1;
    }
    if (*child2 < n && cell.array[*child2] > cell.array[*father]) {
        *father = (*child2)*1;
    }

    if (*father != i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            std::swap(cell.array[*father], cell.array[i]);
        }
        MaxHeapify(cell, *father, n);
    }
}

void HeapSort(Cell& cell) {
    std::atomic<int>* i = &cell.iter1;
    for (*i = cell.array.size()/2 - 1; *i >= 0; (*i)--) {
        MaxHeapify(cell, *i, cell.array.size());
    }

    for (*i = cell.array.size() - 1; *i > 0; (*i)--) {
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            std::swap(cell.array[0], cell.array[*i]);
        }
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        MaxHeapify(cell, 0, *i);
    }
}


void MaxHeapify(std::vector<int>& arr, std::atomic<bool>& isSorting, const int i, const int n) {
    if (!isSorting) return;

    int father = i;
    int child1 = 2*father + 1;
    int child2 = 2*father + 2;

    if (child1 < n && arr[child1] > arr[father]) {
        father = child1;
    }
    if (child2 < n && arr[child2] > arr[father]) {
        father = child2;
    }

    if (father != i) {
        std::swap(arr[father], arr[i]);
        MaxHeapify(arr, isSorting, father, n);
    }
}

void HeapSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    for (int i = arr.size()/2 - 1; i >= 0; i--) {
        MaxHeapify(arr, isSorting, i, arr.size());
    }

    for (int i = arr.size() - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        MaxHeapify(arr, isSorting, 0, i);
    }
}

// COMB SORT
void CombSort(Cell& cell) {
    std::atomic<int>* i = &cell.iter1;
    std::atomic<int>* j = &cell.iter2;

    int n = cell.array.size();
    const double shrinkFactor = 1.247330950103979;
    int gap = n/shrinkFactor > 1 ? n/shrinkFactor : 1;

    int nswaps = n;
    while (nswaps != 0) {
        nswaps = 0;

        for (*i = 0; *i < n - gap; (*i)++) {
            *j = *i + gap;
            if (cell.array[*i] > cell.array[*j]) {
                std::lock_guard<std::mutex> lock(cell.mutex);
                std::swap(cell.array[*i], cell.array[*j]);
                nswaps++;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }

        gap = gap/shrinkFactor > 1 ? gap/shrinkFactor : 1;
    }
}

void CombSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int n = arr.size();
    const double shrinkFactor = 1.247330950103979;
    int gap = n/shrinkFactor > 1 ? n/shrinkFactor : 1;

    int nswaps = n;
    while (nswaps != 0) {
        nswaps = 0;

        for (int i = 0; i < n - gap; i++) {
            if (arr[i] > arr[i + gap]) {
                std::swap(arr[i], arr[i + gap]);
                nswaps++;
            }
            if (!isSorting) return;
        }

        gap = gap/shrinkFactor > 1 ? gap/shrinkFactor : 1;
    }
}

// SHELL SORT
void ShellSort(Cell& cell) {
    int n = cell.array.size();
    const float shrinkFactor = 2.25;
    int gap = n/shrinkFactor;

    while (true) {
        for (int i = 0; i<gap; i++) {
            for (cell.iter1 = i + gap; cell.iter1 < n; cell.iter1 += gap) {
                for (cell.iter2 = (int)cell.iter1; cell.iter2 >= gap; cell.iter2 -= gap) {

                    if (!cell.isSorting) return;
                    std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));

                    if (cell.array[cell.iter2] < cell.array[cell.iter2 - gap]) {
                        std::lock_guard<std::mutex> lock(cell.mutex);
                        std::swap(cell.array[cell.iter2], cell.array[cell.iter2 - gap]);
                    } else {
                        break;
                    }
                }
            }
        }
        if (gap == 1) break;
        gap = gap/shrinkFactor > 1 ? gap/shrinkFactor : 1;
    }
}

void ShellSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int n = arr.size();
    const float shrinkFactor = 2.25;
    int gap = n/shrinkFactor;

    while (true) {
        for (int i = 0; i<gap; i++) {
            for (int j = i + gap; j < n; j += gap) {
                if (!isSorting) return;
                for (int k = j; k >= gap; k -= gap) {
                    if (arr[k] < arr[k - gap]) {
                        std::swap(arr[k], arr[k - gap]);
                    } else {
                        break;
                    }
                }
            }
        }
        if (gap == 1) break;
        gap = gap/shrinkFactor > 1 ? gap/shrinkFactor : 1;
    }
}

// BUCKET SORT
void BucketSort(Cell& cell) {
    std::atomic<int>* i = &cell.iter1;

    int maxValue = cell.array[0];
    int minValue = cell.array[0];
    for (*i = 1; *i < cell.array.size(); (*i)++) {
        if (maxValue < cell.array[*i]) {
            maxValue = cell.array[*i];
        }

        if (minValue > cell.array[*i]) {
            minValue = cell.array[*i];
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    const int bucketsN = std::sqrt(cell.array.size());
    int bSize = (maxValue - minValue + 1)/bucketsN;

    std::vector<std::vector<int>> buckets(bucketsN);

    for (*i = 0; *i < cell.array.size(); (*i)++) {
        int bucketIndx = (cell.array[*i] - minValue)/bSize;
        if (bucketIndx >= bucketsN) bucketIndx = bucketsN - 1;
        buckets[bucketIndx].push_back(cell.array[*i]);

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    *i = 0;
    for (int j = 0; j < bucketsN; j++) {
        for (int k = 0; k < buckets[j].size(); k++) {
            {
                std::lock_guard<std::mutex> lock(cell.mutex);
                cell.array[*i] = buckets[j][k];
            }
            (*i)++;

            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval*20));
        }
    }

    InsertionSort(cell);
}

void BucketSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int maxValue = arr[0];
    int minValue = arr[0];
    for (int i = 1; i < arr.size(); i++) {
        if (maxValue < arr[i]) {
            maxValue = arr[i];
        }

        if (minValue > arr[i]) {
            minValue = arr[i];
        }

        if (!isSorting) return;
    }

    const int bucketsN = std::sqrt(arr.size());
    int bSize = (maxValue - minValue + 1)/bucketsN;

    std::vector<std::vector<int>> buckets(bucketsN);

    for (int i = 0; i < arr.size(); i++) {
        int bucketIndx = (arr[i] - minValue)/bSize;
        if (bucketIndx >= bucketsN) bucketIndx = bucketsN - 1;
        buckets[bucketIndx].push_back(arr[i]);

        if (!isSorting) return;
    }

    int realArrIndx = 0;
    for (int i = 0; i < bucketsN; i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            arr[realArrIndx] = buckets[i][j];
            realArrIndx++;

            if (!isSorting) return;
        }
    }

    InsertionSort(arr, isSorting);
}

// COUNTING SORT
void CountingSort(Cell& cell) {
    int maxValue = cell.array[0];
    std::atomic<int>* i = &cell.iter1;

    for (*i = 1; *i<cell.array.size(); (*i)++) {
        if (maxValue < cell.array[*i]) {
            maxValue = cell.array[*i];
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    std::vector<int> countingVec(maxValue + 1);
    std::vector<int> originalArr = cell.array;

    for (*i = 0; *i<cell.array.size(); (*i)++) {
        countingVec[cell.array[*i]]++;

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    for (int j = 1; j<countingVec.size(); j++) {
        countingVec[j] += countingVec[j-1];

        if (!cell.isSorting) return;
    }

    for (int j = originalArr.size() - 1; j >= 0; j--) {
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            cell.array[countingVec[originalArr[j]] - 1] = originalArr[j];
        }
        countingVec[originalArr[j]]--;

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void CountingSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int maxValue = arr[0];
    for (int i = 1; i<arr.size(); i++) {
        if (maxValue < arr[i]) {
            maxValue = arr[i];
        }
    }

    std::vector<int> countingVec(maxValue + 1);
    std::vector<int> originalArr = arr;

    for (int i = 0; i<arr.size(); i++) {
        countingVec[arr[i]]++;
    }

    for (int i = 1; i<countingVec.size(); i++) {
        countingVec[i] += countingVec[i-1];
    }

    for (int i = originalArr.size() - 1; i >= 0; i--) {
        arr[countingVec[originalArr[i]] - 1] = originalArr[i];
        countingVec[originalArr[i]]--;
    }
}

// RADIX SORT COMMON FUNCS
void OneDigitCountingSort(Cell& cell, int power) {
    std::vector<int> countingVec(10);
    std::vector<int> originalArr = cell.array;

    std::atomic<int>* i = &cell.iter1;
    for (*i = 0; *i<cell.array.size(); (*i)++) {
        countingVec[(cell.array[*i] / (int)pow(10, power)) % 10]++;

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    for (int j = 1; j<countingVec.size(); j++) {
        countingVec[j] += countingVec[j-1];

        if (!cell.isSorting) return;
    }

    for (int j = originalArr.size() - 1; j >= 0; j--) {
        int digit = (originalArr[j] / (int)pow(10, power)) % 10;
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            cell.array[countingVec[digit] - 1] = originalArr[j];
        }
        countingVec[digit]--;

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void OneDigitCountingSort(std::vector<int>& arr, std::atomic<bool>& isSorting, int power) {
    std::vector<int> countingVec(10);
    std::vector<int> originalArr = arr;

    for (int i = 0; i<arr.size(); i++) {
        countingVec[(arr[i] / (int)pow(10, power)) % 10]++;
    }

    for (int i = 1; i<countingVec.size(); i++) {
        countingVec[i] += countingVec[i-1];
    }

    for (int i = originalArr.size() - 1; i >= 0; i--) {
        int digit = (originalArr[i] / (int)pow(10, power)) % 10;
        arr[countingVec[digit] - 1] = originalArr[i];
        countingVec[digit]--;
    }
}

// LSD RADIX SORT
void LSDRadixSort(Cell& cell) {
    int maxValue = cell.array[0];

    std::atomic<int>* i = &cell.iter1;
    for (*i = 1; *i<cell.array.size(); (*i)++) {
        if (maxValue < cell.array[*i]) {
            maxValue = cell.array[*i];
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    for (int p = 0; maxValue / (int)pow(10, p) > 0; p++) {
        OneDigitCountingSort(cell, p);
    }
}

void LSDRadixSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int maxValue = arr[0];
    for (int i = 0; i<arr.size(); i++) {
        if (maxValue < arr[i]) {
            maxValue = arr[i];
        }
    }

    for (int p = 0; maxValue / (int)pow(10, p) > 0; p++) {
        OneDigitCountingSort(arr, isSorting, p);
    }
}

// MSD RADIX SORT
void MSDRadixSortRecursive(Cell& cell, int power, std::vector<int>& subArr, int subArrStartIndx) {
    if (power < 0 || subArr.size() <= 1) return;

    OneDigitCountingSort(subArr, cell.isSorting, power);

    std::vector<std::vector<int>> buckets(10);
    std::atomic<int>* k = &cell.iter2;

    for (int i = 0; i<subArr.size(); i++) {
        buckets[(subArr[i] / (int)pow(10, power)) % 10].push_back(subArr[i]);

        *k = i + subArrStartIndx;

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    int realArrIndx = subArrStartIndx;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            {
                std::lock_guard<std::mutex> lock(cell.mutex);
                cell.array[realArrIndx] = buckets[i][j];
            }
            realArrIndx++;
            *k = realArrIndx;

            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval*3));
        }
    }

    realArrIndx = subArrStartIndx;
    for (int i = 0; i < 10; i++) {
        MSDRadixSortRecursive(cell, power-1, buckets[i], realArrIndx);
        realArrIndx += buckets[i].size();
        if (!cell.isSorting) return;
    }
}

void MSDRadixSort(Cell& cell) {
    int maxValue = cell.array[0];

    std::atomic<int>* i = &cell.iter1;
    for (*i = 1; *i<cell.array.size(); (*i)++) {
        if (maxValue < cell.array[*i]) {
            maxValue = cell.array[*i];
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }

    MSDRadixSortRecursive(cell, std::log10(maxValue), cell.array, 0);
}

void MSDRadixSortRecursive(std::vector<int>& arr, std::atomic<bool>& isSorting, int power, std::vector<int>& subArr, int subArrStartIndx) {
    if (power < 0 || subArr.size() <= 1) return;

    OneDigitCountingSort(subArr, isSorting, power);

    std::vector<std::vector<int>> buckets(10);
    for (int i = 0; i<subArr.size(); i++) {
        buckets[(subArr[i] / (int)pow(10, power)) % 10].push_back(subArr[i]);
        if (!isSorting) return;
    }

    int realArrIndx = subArrStartIndx;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < buckets[i].size(); j++) {
            arr[realArrIndx] = buckets[i][j];
            realArrIndx++;

            if (!isSorting) return;
        }
    }

    realArrIndx = subArrStartIndx;
    for (int i = 0; i < 10; i++) {
        MSDRadixSortRecursive(arr, isSorting, power-1, buckets[i], realArrIndx);
        realArrIndx += buckets[i].size();
        if (!isSorting) return;
    }
}

void MSDRadixSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int maxValue = arr[0];
    for (int i = 0; i<arr.size(); i++) {
        if (maxValue < arr[i]) {
            maxValue = arr[i];
        }

        if (!isSorting) return;
    }

    MSDRadixSortRecursive(arr, isSorting, std::log10(maxValue), arr, 0);
}

// PANCAKE SORT
void PancakeFlip(std::vector<int>& arr, int indx) {
    int i = indx;
    int j = arr.size() - 1;

    while (i < j) {
        std::swap(arr[i], arr[j]);
        i++;
        j--;
    }
}

void PancakeSort(Cell& cell) {
    int arraySize = cell.array.size();
    std::atomic<int>* i = &cell.iter1;
    std::atomic<int>* j = &cell.iter2;

    for (*i = 0; *i<arraySize-1; (*i)++) {
        int minflag = *i;
        for (*j = *i + 1; *j<arraySize; (*j)++) {
            if (cell.array[minflag] > cell.array[*j]) {
                minflag = *j;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }

        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            PancakeFlip(cell.array, minflag);
        }
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval*arraySize / 2));


        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            PancakeFlip(cell.array, *i);
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void PancakeSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int arraySize = arr.size();
    for (int i = 0; i<arraySize-1; i++) {
        int minflag = i;
        for (int j = i + 1; j<arraySize; j++) {
            if (arr[minflag] > arr[j]) {
                minflag = j;
            }
        }

        PancakeFlip(arr, minflag);
        PancakeFlip(arr, i);

        if (!isSorting) return;
    }
}

// CYCLE SORT
void CycleSort(Cell& cell) {
    int swaps = 0;
    int savedValue = 0;
    int currentValue = cell.array[0];

    std::atomic<int>* i = &cell.iter1;
    std::atomic<int>* j = &cell.iter2;

    while (swaps < cell.array.size()) {
        *j = 0;
        for (*i = 0; *i<cell.array.size(); (*i)++) {
            if (cell.array[*i] < currentValue) {
                (*j)++;
            }
            if (!cell.isSorting) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
        }

        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            savedValue = cell.array[*j];
            cell.array[*j] = currentValue;
            currentValue = savedValue;
        }
        swaps++;

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void CycleSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    int swaps = 0;
    int savedValue = 0;
    int currentValue = arr[0];

    while (swaps < arr.size()) {
        int nSmaller = 0;
        for (int i = 0; i<arr.size(); i++) {
            if (arr[i] < currentValue) {
                nSmaller++;
            }
            if (!isSorting) return;
        }
        savedValue = arr[nSmaller];
        arr[nSmaller] = currentValue;
        currentValue = savedValue;
        swaps++;

        if (!isSorting) return;
    }
}

// >> ESOTERIC ALGORITHMS <<

// BOGO SORT
void BogoSort(Cell& cell) {
    while (!isSorted(cell.array)) {
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            ShufflePro(cell.array);
        }
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void BogoSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    while (!isSorted(arr)) {
        ShufflePro(arr);
        if (!isSorting) return;
    }
}

// STALIN SORT
void StalinSort(Cell& cell) {
    for (cell.iter1 = 1; cell.iter1<cell.array.size(); cell.iter1++) {
        if (cell.array[cell.iter1 - 1] > cell.array[cell.iter1]) {
            {
                std::lock_guard<std::mutex> lock(cell.mutex);
                cell.array.erase(cell.array.begin() + cell.iter1);
            }
            cell.iter1--;
        }
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void StalinSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    std::vector<int> stalinSorted = { arr[0] };

    for (int i = 1; i<arr.size(); i++) {
        if (arr[i] >= stalinSorted.back()) {
            stalinSorted.push_back(arr[i]);
        }
        if (!isSorting) return;
    }

    arr = stalinSorted;
}

// SLEEP SORT
void SleepThread(std::vector<int>& arr, std::atomic<bool>& isSorting, int n, int ms, unsigned long long int offset) {
    unsigned long long int time = offset;
    while (time < ms*n) {
        time++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        if (!isSorting) return;
    }
    arr.push_back(n);
}

void SleepSort(Cell& cell) {
    std::vector<std::thread> sleepThreads;

    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i<cell.array.size(); i++) {
        auto now = std::chrono::high_resolution_clock::now();
        auto offset = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
        sleepThreads.push_back(std::thread(SleepThread, std::ref(cell.array), std::ref(cell.isSorting),
            cell.array[i], cell.interval, offset.count()));
    }
    cell.array.clear();

    for (int i = 0; i<sleepThreads.size(); i++) {
        sleepThreads[i].join();
    }
    sleepThreads.clear();
}

void SleepSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    std::vector<std::thread> sleepThreads;

    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i<arr.size(); i++) {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> offset = now - startTime;
        sleepThreads.push_back(std::thread(SleepThread, std::ref(arr), std::ref(isSorting), arr[i], 1, offset.count()));
    }
    arr.clear();

    for (int i = 0; i<sleepThreads.size(); i++) {
        sleepThreads[i].join();
    }
    sleepThreads.clear();
}

// MIRACLE SORT
void MiracleSort(Cell& cell) {
    while (!isSorted(cell.array)) {
        if (!cell.isSorting) return;
    }
}

void MiracleSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    while (!isSorted(arr)) {
        if (!isSorting) return;
    }
}

// SNOW FLAGE SORT
float StalinCheck(std::vector<int>& arr) {
    std::vector<int> stalinSorted = { arr[0] };

    for (int i = 1; i<arr.size(); i++) {
        if (arr[i] >= stalinSorted.back()) {
            stalinSorted.push_back(arr[i]);
        }
    }

    return stalinSorted.size()/(float)(arr.size());
}

void SnowFlakeSort(Cell& cell) {
    float solvedPercentage = StalinCheck(cell.array);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, cell.array.size() - 1);

    while (solvedPercentage != 1) {
        cell.iter1 = dis(gen);
        cell.iter2 = dis(gen);

        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            std::swap(cell.array[cell.iter1], cell.array[cell.iter2]);
        }

        float newSolvedPercentage = StalinCheck(cell.array);
        if (newSolvedPercentage >= solvedPercentage) {
            solvedPercentage = newSolvedPercentage;
        } else {
            std::swap(cell.array[cell.iter1], cell.array[cell.iter2]);
        }
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void SnowFlakeSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    float solvedPercentage = StalinCheck(arr);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, arr.size() - 1);

    while (solvedPercentage != 1) {
        int i = dis(gen);
        int j = dis(gen);

        std::swap(arr[i], arr[j]);

        float newSolvedPercentage = StalinCheck(arr);
        if (newSolvedPercentage >= solvedPercentage) {
            solvedPercentage = newSolvedPercentage;
        } else {
            std::swap(arr[i], arr[j]);
        }
        if (!isSorting) return;
    }
}

// BOZO SORT
void BozoSort(Cell& cell) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, cell.array.size() - 1);

    while (!isSorted(cell.array)) {
        cell.iter1 = dis(gen);
        cell.iter2 = dis(gen);

        if (cell.iter1 == cell.iter2) continue;

        if (cell.iter1 < cell.iter2) {
            if (cell.array[cell.iter1] > cell.array[cell.iter2]) {
                std::lock_guard<std::mutex> lock(cell.mutex);
                std::swap(cell.array[cell.iter1], cell.array[cell.iter2]);
            }
        } else {
            if (cell.array[cell.iter2] > cell.array[cell.iter1]) {
                std::lock_guard<std::mutex> lock(cell.mutex);
                std::swap(cell.array[cell.iter1], cell.array[cell.iter2]);
            }
        }

        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void BozoSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, arr.size() - 1);

    while (!isSorted(arr)) {
        int i = dis(gen);
        int j = dis(gen);

        if (i == j) continue;

        if (i < j) {
            if (arr[i] > arr[j]) {
                std::swap(arr[i], arr[j]);
            }
        } else {
            if (arr[j] > arr[i]) {
                std::swap(arr[i], arr[j]);
            }
        }

        if (!isSorting) return;
    }
}
