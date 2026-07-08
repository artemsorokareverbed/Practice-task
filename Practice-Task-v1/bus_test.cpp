#include <QObject>
#include <QtTest>
#include "bus_logic.h"

class BusLogicTest : public QObject {
    Q_OBJECT

private slots:

    void testHoareSortAsc() {
        QList<BusTrip> list;
        list.append({100, 3600, 20260701});
        list.append({11, 7200, 20260702});
        list.append({55, 1200, 20260703});


        hoareQuickSort(list, 0, list.size() - 1, true);


        QCOMPARE(list[0].number, 11);
        QCOMPARE(list[1].number, 55);
        QCOMPARE(list[2].number, 100);
    }


    void testHoareSortDesc() {
        QList<BusTrip> list;
        list.append({100, 3600, 20260701});
        list.append({11, 7200, 20260702});
        list.append({55, 1200, 20260703});

        hoareQuickSort(list, 0, list.size() - 1, false);

        QCOMPARE(list[0].number, 100);
        QCOMPARE(list[1].number, 55);
        QCOMPARE(list[2].number, 11);
    }


    void testEmptyList() {
        QList<BusTrip> list;


        hoareQuickSort(list, 0, list.size() - 1, true);

        QVERIFY(list.isEmpty());
    }
};


QTEST_MAIN(BusLogicTest)
#include "bus_test.moc"
