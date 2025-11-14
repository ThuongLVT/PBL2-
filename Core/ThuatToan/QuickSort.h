#ifndef QUICKSORT_H
#define QUICKSORT_H
#include <string>

#include "../CauTrucDuLieu/MangDong.h"

// Template class QuickSort
template <typename T>
class QuickSort
{
private:
    // Hàm partition
    static int partition(MangDong<T> &arr, int low, int high, bool (*compare)(const T &, const T &))
    {
        T pivot = arr[high];
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (compare(arr[j], pivot))
            {
                i++;
                T temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        T temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        return i + 1;
    }

    // Hàm đệ quy quicksort
    static void quickSortRecursive(MangDong<T> &arr, int low, int high, bool (*compare)(const T &, const T &))
    {
        if (low < high)
        {
            int pi = partition(arr, low, high, compare);
            quickSortRecursive(arr, low, pi - 1, compare);
            quickSortRecursive(arr, pi + 1, high, compare);
        }
    }

public:
    // Hàm sắp xếp chính
    static void sort(MangDong<T> &arr, bool (*compare)(const T &, const T &))
    {
        if (arr.size() <= 1)
            return;
        quickSortRecursive(arr, 0, arr.size() - 1, compare);
    }

    // Hàm sắp xếp tăng dần (mặc định)
    static void sortAscending(MangDong<T> &arr)
    {
        sort(arr, [](const T &a, const T &b)
             { return a < b; });
    }

    // Hàm sắp xếp giảm dần
    static void sortDescending(MangDong<T> &arr)
    {
        sort(arr, [](const T &a, const T &b)
             { return a > b; });
    }
};

#endif // QUICKSORT_H
