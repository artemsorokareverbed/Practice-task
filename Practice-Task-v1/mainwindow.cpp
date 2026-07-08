#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIcon>
#include <QMenu>
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    maxArraySize = 100;

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *topLimitLayout = new QHBoxLayout();
    QLabel *lblLimitTitle = new QLabel("Максимальный размер массива:", this);
    leMaxSize = new QLineEdit("100", this);
    leMaxSize->setFixedWidth(80);
    btnApplySize = new QPushButton("Применить размер", this);

    topLimitLayout->addWidget(lblLimitTitle);
    topLimitLayout->addWidget(leMaxSize);
    topLimitLayout->addWidget(btnApplySize);
    topLimitLayout->addStretch();

    lblSize = new QLabel("Размер массива (кол-во рейсов): 0", this);
    QFont font = lblSize->font();
    font.setBold(true);
    font.setPointSize(11);
    lblSize->setFont(font);

    tableWidget = new QTableWidget(0, 4, this);
    tableWidget->setHorizontalHeaderLabels({"Иконка", "Номер", "Длительность", "Дата"});
    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    listWidget = new QListWidget(this);
    listView = new QListView(this);
    listViewModel = new QStringListModel(this);
    listView->setModel(listViewModel);

    btnAdd = new QPushButton("Добавить пустой [Ctrl+N]", this);
    btnAdd->setShortcut(QKeySequence("Ctrl+N"));

    btnRemove = new QPushButton("Удалить рейс [Del]", this);

    btnLoad = new QPushButton("Загрузить [Ctrl+O]", this);
    btnLoad->setShortcut(QKeySequence("Ctrl+O"));

    btnSortHoareAsc = new QPushButton("Хоара Возр. [Ctrl+1]", this);
    btnSortHoareAsc->setShortcut(QKeySequence("Ctrl+1"));

    btnSortHoareDesc = new QPushButton("Хоара Убыв. [Ctrl+2]", this);
    btnSortHoareDesc->setShortcut(QKeySequence("Ctrl+2"));

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnRemove);
    btnLayout->addWidget(btnLoad);
    btnLayout->addWidget(btnSortHoareAsc);
    btnLayout->addWidget(btnSortHoareDesc);

    mainLayout->addLayout(topLimitLayout);
    mainLayout->addWidget(lblSize);
    mainLayout->addWidget(tableWidget);

    mainLayout->addWidget(new QLabel("Компонент: QListWidget", this));
    mainLayout->addWidget(listWidget);

    mainLayout->addWidget(new QLabel("Компонент: QListView", this));
    mainLayout->addWidget(listView);

    mainLayout->addLayout(btnLayout);

    setCentralWidget(centralWidget);
    resize(900, 700);

    connect(btnApplySize, &QPushButton::clicked, this, &MainWindow::applyMaxSize);
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::addBus);
    connect(btnRemove, &QPushButton::clicked, this, &MainWindow::removeBus);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::loadFromFile);
    connect(btnSortHoareAsc, &QPushButton::clicked, this, &MainWindow::sortHoareAsc);
    connect(btnSortHoareDesc, &QPushButton::clicked, this, &MainWindow::sortHoareDesc);
    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

    connect(tableWidget, &QTableWidget::cellChanged, this, [this](int row, int column) {
        if (row >= busList.size()) return;
        QTableWidgetItem *item = tableWidget->item(row, column);
        if (!item) return;

        QString newValue = item->text();
        if (column == 1) busList[row].number = newValue.toInt();
        else if (column == 2) busList[row].duration = newValue.toLongLong();
        else if (column == 3) busList[row].date = newValue.toLongLong();

        tableWidget->blockSignals(true);
        updateViews();
        tableWidget->blockSignals(false);
    });
}

MainWindow::~MainWindow() {}

void MainWindow::applyMaxSize() {
    int newMax = leMaxSize->text().toInt();
    if (newMax <= 0) {
        QMessageBox::warning(this, "Ошибка", "Размер должен быть больше нуля!");
        return;
    }

    if (busList.size() > newMax) {
        QMessageBox::warning(this, "Ошибка", "Текущий массив больше нового лимита. Лишние элементы будут отрезаны.");
        while (busList.size() > newMax) {
            busList.removeLast();
        }
        updateViews();
    }

    maxArraySize = newMax;
    QMessageBox::information(this, "Успех", "Максимальный размер успешно изменен на: " + QString::number(maxArraySize));
}

void MainWindow::addBus() {
    if (busList.size() >= maxArraySize) {
        QMessageBox::warning(this, "Ошибка", "Достигнут максимальный предел массива!");
        return;
    }
    BusTrip trip = {0, 0, 0};
    busList.append(trip);
    updateViews();
}

void MainWindow::removeBus() {
    int currentRow = tableWidget->currentRow();
    if (currentRow >= 0 && currentRow < busList.size()) {
        busList.removeAt(currentRow);
    } else if (!busList.isEmpty()) {
        busList.removeLast();
    }
    updateViews();
}

void MainWindow::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Text Files (*.txt)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream in(&file);
    if (in.atEnd()) return;

    QString sizeStr = in.readLine();
    int count = sizeStr.toInt();

    if (count > maxArraySize) {
        QMessageBox::critical(this, "Ошибка", QString("В файле указано %1 элементов, а лимит всего %2! Загрузка отменена.").arg(count).arg(maxArraySize));
        file.close();
        return;
    }

    busList.clear();

    for (int i = 0; i < count && !in.atEnd(); ++i) {
        QString line = in.readLine();
        QStringList parts = line.split(" ", Qt::SkipEmptyParts);

        if (parts.size() >= 3) {
            BusTrip trip;
            trip.number = parts[0].toInt();
            trip.duration = parts[1].toLongLong();
            trip.date = parts[2].toLongLong();
            busList.append(trip);
        }
    }
    file.close();
    updateViews();
}

void MainWindow::updateViews() {
    tableWidget->blockSignals(true);
    tableWidget->setRowCount(0);
    listWidget->clear();
    QStringList stringList;

    lblSize->setText(QString("Размер массива (кол-во рейсов): %1 из %2").arg(busList.size()).arg(maxArraySize));

    for (int i = 0; i < busList.size(); ++i) {
        const auto& trip = busList[i];
        tableWidget->insertRow(i);

        QTableWidgetItem *iconItem = new QTableWidgetItem();
        iconItem->setIcon(QIcon(":/bus.png"));
        iconItem->setFlags(iconItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(i, 0, iconItem);

        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(trip.number)));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(trip.duration)));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(trip.date)));

        QString text = QString("Рейс №%1 | %2 сек | Дата: %3").arg(trip.number).arg(trip.duration).arg(trip.date);

        listWidget->addItem(new QListWidgetItem(QIcon(":/bus.png"), text));
        stringList << text;
    }
    listViewModel->setStringList(stringList);
    tableWidget->blockSignals(false);
}

void MainWindow::sortHoareAsc() {
    if (busList.isEmpty()) return;
    hoareQuickSort(busList, 0, busList.size() - 1, true);
    updateViews();
}

void MainWindow::sortHoareDesc() {
    if (busList.isEmpty()) return;
    hoareQuickSort(busList, 0, busList.size() - 1, false);
    updateViews();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) removeBus();
    QMainWindow::keyPressEvent(event);
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QMenu contextMenu(this);
    QAction actionDelete("Удалить рейс", this);
    connect(&actionDelete, &QAction::triggered, this, &MainWindow::removeBus);
    contextMenu.addAction(&actionDelete);
    contextMenu.exec(tableWidget->mapToGlobal(pos));
}
