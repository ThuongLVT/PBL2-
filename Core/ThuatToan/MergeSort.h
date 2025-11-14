#ifndef MERGESORT_H
#define MERGESORT_H
#include <string>

#include "../CauTrucDuLieu/MangDong.h"

// Template class MergeSort
template <typename T>
class MergeSort
{
private:
    // Hàm merge hai mảng con
    static void merge(MangDong<T> &arr, int left, int mid, int right, bool (*compare)(const T &, const T &))
    {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        // Tạo mảng tạm
        MangDong<T> L, R;
        for (int i = 0; i < n1; i++)
        {
            L.push_back(arr[left + i]);
        }
        for (int j = 0; j < n2; j++)
        {
            R.push_back(arr[mid + 1 + j]);
        }

        // Merge
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2)
        {
            if (compare(L[i], R[j]))
            {
                arr[k] = L[i];
                i++;
            }
            else
            {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        // Copy các phần tử còn lại
        while (i < n1)
        {
            arr[k] = L[i];
            i++;
            k++;
        }
        while (j < n2)
        {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    // Hàm đệ quy mergesort
    static void mergeSortRecursive(MangDong<T> &arr, int left, int right, bool (*compare)(const T &, const T &))
    {
        if (left < right)
        {
            int mid = left + (right - left) / 2;
            mergeSortRecursive(arr, left, mid, compare);
            mergeSortRecursive(arr, mid + 1, right, compare);
            merge(arr, left, mid, right, compare);
        }
    }

public:
    // Hàm sắp xếp chính
    static void sort(MangDong<T> &arr, bool (*compare)(const T &, const T &))
    {
        if (arr.size() <= 1)
            return;
        mergeSortRecursive(arr, 0, arr.size() - 1, compare);
    }

    // Hàm sắp xếp tăng dần (mặc định)
    static void sortAscending(MangDong<T> &arr)
    {
        sort(arr, [](const T &a, const T &b)
             { return a <= b; });
    }

    // Hàm sắp xếp giảm dần
    static void sortDescending(MangDong<T> &arr)
    {
        sort(arr, [](const T &a, const T &b)
             { return a >= b; });
    }
};

#endif // MERGESORT_H
