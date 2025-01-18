#include "SortingAlgorithms.h"

// SHUFFLE
void Shuffle(std::vector<int>& arr) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(arr.begin(), arr.end(), gen);
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
    int n = cell.array.size();
    while (n > 1) {
        int lastSwap = 0;

        for (cell.iter1 = 0, cell.iter2 = 1; cell.iter1 < n - 1; cell.iter1++, cell.iter2++) {
            if (cell.array[cell.iter1] > cell.array[cell.iter1 + 1]) {
                std::lock_guard<std::mutex> lock(cell.mutex);
                std::swap(cell.array[cell.iter1], cell.array[cell.iter2]);
                lastSwap = cell.iter1 + 1;
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

        for (int i = 0; i < n - 1; i++) {
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

            std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
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

void BinaryInsertionSort(std::vector<int>& arr, std::atomic<bool>& isSorting){
    for (int i = 1; i<arr.size(); i++) {
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
        arr.insert(arr.begin() + i, key);
    }
}


// >> ESOTERIC ALGORITHMS <<

// BOGO SORT
void BogoSort(Cell& cell) {
    while (!isSorted(cell.array)) {
        {
            std::lock_guard<std::mutex> lock(cell.mutex);
            Shuffle(cell.array);
        }
        if (!cell.isSorting) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(cell.interval));
    }
}

void BogoSort(std::vector<int>& arr, std::atomic<bool>& isSorting) {
    while (!isSorted(arr)) {
        Shuffle(arr);
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

        if (cell.iter1 < cell.iter2){
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
