#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP
#include "vetor.hpp"

template <typename T, typename Compare>
class MergeSort {
public:
    static void sort(VetorDinamico<T>& vec, Compare comp) {
        if (vec.size() > 1) {
            mergeSort(vec, 0, vec.size() - 1, comp);
        }
    }

private:
    static void merge(VetorDinamico<T>& vec, int left, int mid, int right, Compare comp) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        VetorDinamico<T> L(n1);
        VetorDinamico<T> R(n2);

        for (int i = 0; i < n1; i++)
            L[i] = vec[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = vec[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            if (comp(L[i], R[j])) {
                vec[k] = L[i];
                i++;
            } else {
                vec[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            vec[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            vec[k] = R[j];
            j++;
            k++;
        }
    }

    static void mergeSort(VetorDinamico<T>& vec, int left, int right, Compare comp) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(vec, left, mid, comp);
            mergeSort(vec, mid + 1, right, comp);
            merge(vec, left, mid, right, comp);
        }
    }
};

#endif
