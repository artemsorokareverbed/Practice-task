#ifndef BUS_LOGIC_H
#define BUS_LOGIC_H

#include <QtGlobal>
#include <QList>
#include <utility>

struct BusTrip {
    qint8 number;
    qlonglong duration;
    qint64 date;
};

inline int partition(QList<BusTrip>& arr, int low, int high, bool ascending) {
    qint8 pivot = arr[(low + high) / 2].number;
    int i = low - 1;
    int j = high + 1;
    while (true) {
        if (ascending) {
            do { i++; } while (arr[i].number < pivot);
            do { j--; } while (arr[j].number > pivot);
        } else {
            do { i++; } while (arr[i].number > pivot);
            do { j--; } while (arr[j].number < pivot);
        }
        if (i >= j) return j;
        std::swap(arr[i], arr[j]);
    }
}

inline void hoareQuickSort(QList<BusTrip>& arr, int low, int high, bool ascending) {
    if (low < high) {
        int pi = partition(arr, low, high, ascending);
        hoareQuickSort(arr, low, pi, ascending);
        hoareQuickSort(arr, pi + 1, high, ascending);
    }
}

#endif // BUS_LOGIC_H
