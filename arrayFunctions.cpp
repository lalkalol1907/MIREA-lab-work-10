//
// Created by Konstantin on 08.12.2022.
//

#include <random>
#include <algorithm>
#include "linkedList.cpp"

bool equal(double a, double b) {
    return fabs(a - b) < 1e-300;
}

namespace arrayFunctions {

    template<typename T>
    T max(T arr[], int n) {
        if (arr == nullptr)
            return 0;

        T _max = arr[0];
        for (int i = 0; i < n; i++) {
            if (arr[i] > _max) {
                _max = arr[i];
            }
        }
        return _max;
    }

    template<typename T>
    T min(T arr[], int n) {
        if (arr == nullptr)
            return 0;

        T _min = arr[0];
        for (int i = 0; i < n; i++) {
            if (arr[i] < _min) {
                _min = arr[i];
            }
        }

        return _min;
    }

    template <typename T>
    double avg(T arr[], int n) {
        if (arr == nullptr)
            return 0;

        double sum = 0;
        for (int i = 0; i < n; i++)
            sum += arr[i];
        return sum / n;
    }

    template<typename T>
    int quickSortPartition(T arr[], int start, int end) {
        T pivot = arr[start];
        int count = 0;

        for (int i = start + 1; i <= end; i++) {
            if (arr[i] <= pivot)
                count++;
        }

        int pivotIndex = start + count;
        std::swap(arr[pivotIndex], arr[start]);

        int i = start, j = end;

        while (i < pivotIndex && j > pivotIndex) {
            while (arr[i] <= pivot) {
                i++;
            }

            while (arr[j] > pivot) {
                j--;
            }

            if (i < pivotIndex && j > pivotIndex) {
                std::swap(arr[i++], arr[j--]);
            }
        }

        return pivotIndex;
    }

    template<typename T>
    void quickSort(T arr[], int start, int end) {
        if (start >= end)
            return;

        int p = quickSortPartition(arr, start, end);
        quickSort(arr, start, p - 1);
        quickSort(arr, p + 1, end);
    }

    template<typename T>
    void quickSort(T arr[], int n) {
        if (arr == nullptr)
            return;

        quickSort(arr, 0, n - 1);
    }

    template<typename T>
    bool sorted(T arr[], int n) {
        if (arr == nullptr)
            return false;

        for (int i = 0; i < n - 1; i++) {
            if (arr[i + 1] < arr[i])
                return false;
        }
        return true;
    }


    template<typename T>
    bool randomSort(T arr[], int n, int seconds) {

        if (arr == nullptr)
            return false;

        auto startTime = time(nullptr);

        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<int> dist(0, n - 1);
        int a, b;
        while (!sorted(arr, n)) {
            a = dist(rng);
            b = dist(rng);

            if (a == b)
                continue;

            if (time(nullptr) - startTime > seconds)
                return false;

            std::swap(arr[a], arr[b]);
        }

        return true;
    }

    template<typename T>
    void bubbleSort(T arr[], int n) {
        if (arr == nullptr)
            return;

        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                if (arr[j] < arr[i]) {
                    std::swap(arr[i], arr[j]);
                }
            }
        }
    }

    template<typename T>
    void combSort(T arr[], int n) {
        if (arr == nullptr)
            return;

        double fi = 1.247;
        int step = n - 1;
        while (step >= 1) {
            for (int i = 0; i + step < n; i++) {
                if (arr[i] > arr[i + step]) {
                    std::swap(arr[i], arr[i + step]);
                }
            }
            step /= fi;
        }
    }

    template<typename T>
    void gnomeSort(T arr[], int n) {
        if (arr == nullptr)
            return;

        int i = 1;
        while (i < n) {
            if (i == 0 || arr[i - 1] <= arr[i]) {
                i++;
            } else {
                std::swap(arr[i], arr[i - 1]);
                i--;
            }
        }
    }

    template<typename T>
    Node* binaryFind(T arr[], int n, T e) {
        int left = 0;
        int right = n - 1;
        int index = -1;
        while (left <= right) {
            int mid = (left + right) / 2;
            if (equal(e, arr[mid])) {
                index = mid;
                break;
            }
            if (e < arr[mid])
                right = mid - 1;
            else
                left = mid + 1;
        }

        if (index == -1)
            return new Node(-1);

        int i = index + 1;
        Node* head = new Node(index);
        Node* cur = head;
        while (equal(arr[i], e) && i < n) {
            cur->next = new Node(i++);
            cur = cur->next;
        }

        i = index - 1;
        while(equal(arr[i], e) && i > -1) {
            cur->next = new Node(i--);
            cur = cur->next;
        }

        return mergeSort(head);
    }

    template<typename T>
    Node* find(T arr[], int n, T e) {
        Node* head = new Node(-1);
        Node* cur = head;
        for (int i = 0; i < n; i++) {
            if (arr[i] != e)
                continue;

            if (cur->data == -1) {
                cur->data = i;
                continue;
            }

            cur->next = new Node(i);
            cur = cur->next;
        }
        return head;
    }

    template <typename T>
    T * resize(T *array, int oldSize, int newSize){
        T *newArray = new T[newSize];
        memcpy(newArray, array, sizeof(T) * std::min(oldSize, newSize));
        delete [] array;
        return newArray;
    }

    template <typename T>
    T * deleteDuplicates(T *arr, int size, int * newSize) {
        int counter = 1;
        T * newArr = new T[size];
        T lastElem = arr[0];
        newArr[0] = arr[0];
        for (int i = 1; i < size; i++) {
            if (arr[i] > lastElem) {
                newArr[counter++] = arr[i];
                lastElem = arr[i];
            }
        }

        delete [] arr;
        *newSize = counter;
        return arrayFunctions::resize(newArr, size, counter);
    }
}
