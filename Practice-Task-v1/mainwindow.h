#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QListWidget>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include "bus_logic.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void addBus();
    void removeBus();
    void loadFromFile();
    void sortHoareAsc();
    void sortHoareDesc();
    void showContextMenu(const QPoint &pos);
    void applyMaxSize();

private:
    void updateViews();

    QList<BusTrip> busList;
    int maxArraySize;

    QTableWidget *tableWidget;
    QListWidget *listWidget;
    QListView *listView;
    QStringListModel *listViewModel;

    QLabel *lblSize;
    QLineEdit *leMaxSize;
    QPushButton *btnApplySize;

    QPushButton *btnAdd;
    QPushButton *btnRemove;
    QPushButton *btnLoad;
    QPushButton *btnSortHoareAsc;
    QPushButton *btnSortHoareDesc;
};

#endif
