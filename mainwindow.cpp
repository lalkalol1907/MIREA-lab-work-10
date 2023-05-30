#include "mainwindow.h"
#include "arrayFunctions.cpp"

int rows = 0;
double *arr = new double[0];
bool *inited = new bool[0];
bool sorted = false;
bool finding = false;

Node *findRes = new Node(-1);

template<typename T>
bool all(T *iterable, int n) {
    for (int i = 0; i < n; i++)
        if (!iterable[i])
            return false;
    return true;
}

void colorize(QTableWidgetItem *item, bool c) {
    if (item == nullptr)
        return;
    QColor color = c ? QColor::fromRgb(200, 50, 50) : QColor::fromRgb(50, 200, 50);
    item->setBackground(color);
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->inputTable->setColumnCount(1);
    changeButtonsStatus(false);
    ui->bubbleSort->setChecked(true);
    ui->max->hide();
    ui->min->hide();
    ui->avg->hide();
    ui->findMethodLabel->hide();
    ui->indexesLabel->hide();
}

double timeOf() {
    clock_t tStart = clock();
    double *arr = new double[1000];
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < 1000; j++) {
            std::swap(arr[i], arr[j]);
        }
    }
    delete[] arr;
    return (double) (clock() - tStart) / CLOCKS_PER_SEC / 1000000;
}

double timeOfIteration = timeOf();

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::confirmation() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Вопросик провентилируй");
    msgBox.setText("Ты уверен, что хочешь начать эту операцию? Она может быть оооооочень долгой, как повезет :)");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    return msgBox.exec() == QMessageBox::Yes;
}

void MainWindow::clearOutput() {
    ui->max->hide();
    ui->min->hide();
    ui->avg->hide();
    ui->findMethodLabel->hide();
    ui->indexesLabel->hide();
}

void MainWindow::changeButtonsStatus(bool b) {
    ui->maxButton->setDisabled(!b);
    ui->minButton->setDisabled(!b);
    ui->avgButton->setDisabled(!b);
    ui->sortButton->setDisabled(!b);
    ui->find->setDisabled(!b);
    ui->deleteDup->setDisabled(!sorted);
}

void MainWindow::fillTable(double *arr, int n) {
    for (int i = 0; i < n; i++) {
        auto item = new QTableWidgetItem;
        item->setText(QString::number(arr[i]));
        item->setBackground(QColor::fromRgb(50, 200, 50));
        ui->inputTable->setItem(i, 0, item);
    }
}

void MainWindow::on_clear_pressed() {
    ui->inputTable->setRowCount(0);
    ui->inputTable->setRowCount(rows);
    for (int i = 0; i < rows; i++) {
        inited[i] = false;
    }
    clearOutput();
    changeButtonsStatus(false);
}

void MainWindow::on_random_pressed() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<double> dist(-100, 100);
    sorted = false;
    for (int i = 0; i < rows; i++) {
        arr[i] = dist(rng);
        inited[i] = true;
        auto ni = new QTableWidgetItem;
        ni->setText(QString::number(arr[i]));
        ui->inputTable->setItem(i, 0, ni);
    }
    changeButtonsStatus(all(inited, rows));
    clearOutput();
}

void MainWindow::on_elemFind_textEdited(const QString &arg1) {
    bool flag;
    auto a = arg1.toDouble(&flag);
    flag = flag && !qIsNaN(a) && !qIsInf(a);
    ui->find->setDisabled(!all(arr, rows) || !flag);
}


void MainWindow::on_find_pressed() {
    auto elem = ui->elemFind->text().toDouble();
    Node *result = sorted ? arrayFunctions::binaryFind(arr, rows, elem) : arrayFunctions::find(arr, rows, elem);
    ui->findMethodLabel->setText(QString("Метод поиска: ") + QString(sorted ? "Бинарный" : "Перебор"));
    ui->findMethodLabel->show();
    finding = true;
    for (int i = 0; i < rows; i++)
        colorize(ui->inputTable->item(i, 0), false);
    if (result->data == -1) {
        ui->indexesLabel->setText("Ничего не найдено");
        ui->indexesLabel->show();
        finding = false;
        return;
    }

    Node *cur = result;
    auto count = 0;
    QString strings = "";
    while (cur) {
        count++;
        strings += QString::number(cur->data + 1);
        if (cur->next) strings += ", ";
        QTableWidgetItem *item = ui->inputTable->item(cur->data, 0);
        if (item != nullptr)
            item->setBackground(Qt::blue);
        cur = cur->next;
    }

    ui->indexesLabel->setText(
            QString("Найдено ") + QString::number(count) + QString(" элементов в строках: ") + strings);
    ui->indexesLabel->show();

    finding = false;
    findRes = result;
}


void MainWindow::on_size_textEdited(const QString &arg1) {
    bool flag;
    auto a = arg1.toDouble(&flag);
    if (!flag || a != rows) {
        changeButtonsStatus(false);
        ui->random->setDisabled(true);
        return;
    }
    changeButtonsStatus(!all(arr, rows));
}


void MainWindow::on_sizeButton_pressed() {
    bool flag;
    auto text = ui->size->text();
    int r = text.toInt(&flag);
    if (!flag || r < 1 || r > 1000000) {
        QMessageBox::warning(this, "Ошибка", "Некорректное значение!");
        return;
    }

    arr = arrayFunctions::resize(arr, rows, r);
    inited = arrayFunctions::resize(inited, rows, r);
    rows = r;
    ui->inputTable->setRowCount(rows);

    auto cur = findRes;
    while (cur) {
        if (cur->data == -1) break;
        colorize(ui->inputTable->item(cur->data, 0), false);
        cur = cur->next;
    }
    ui->random->setDisabled(false);
    changeButtonsStatus(all(inited, rows));
    clearOutput();
    ui->findMethodLabel->hide();
    ui->indexesLabel->hide();
}


void MainWindow::on_inputTable_itemChanged(QTableWidgetItem *item) {
    if (finding)
        return;
    auto cur = findRes;
    while (cur) {
        if (cur->data == -1) break;
        colorize(ui->inputTable->item(cur->data, 0), false);
        cur = cur->next;
    }
    bool flag;
    double val = item->text().toDouble(&flag);
    inited[item->row()] = false;
    ui->findMethodLabel->hide();
    ui->indexesLabel->hide();
    clearOutput();

    if (!flag || qIsInf(val) || qIsNaN(val)) {
        changeButtonsStatus(false);
        colorize(item, true);
        return;
    }

    arr[item->row()] = val;
    inited[item->row()] = true;
    colorize(item, false);
    changeButtonsStatus(all(inited, rows));

    if (sorted) {
        int i = item->row();
        if (i == rows - 1) {
            if (rows - 2 < 0) {
                sorted = false;
                return;
            }
            sorted = arr[i] >= arr[i - 1];
            return;
        }

        if (i == 0) {
            if (i + 1 >= rows) {
                sorted = false;
                return;
            }
            sorted = arr[i] <= arr[i + 1];
            return;
        }

        if (i - 1 > -1 && i + 1 < rows) {
            sorted = arr[i - 1] <= arr[i] && arr[i] <= arr[i + 1];
            return;
        }

        sorted = false;
    }
}


void MainWindow::on_sortButton_pressed() {
    if (ui->randomSort->isChecked()) {
        if (!confirmation())
            return;

        if (!arrayFunctions::randomSort(arr, rows, 20)) {
            finding = true;
            fillTable(arr, rows);
            finding = false;
            QMessageBox::warning(this, "Ошибка", "Ошибка сортировки брат, обезьяна не смогла");
            return;

        }
    }

    if (ui->quickSort->isChecked()) {
        if (timeOfIteration * rows * log(rows) > 3 && !confirmation()) {
            return;
        }
        arrayFunctions::quickSort(arr, rows);
    }


    if (ui->gnomeSort->isChecked()) {
        if (timeOfIteration * rows * rows > 3 && !confirmation()) {
            return;
        }
        arrayFunctions::gnomeSort(arr, rows);
    }

    if (ui->bubbleSort->isChecked()) {
        if (timeOfIteration * rows * rows > 3 && !confirmation()) {
            return;
        }
        arrayFunctions::bubbleSort(arr, rows);

    }

    if (ui->combSort->isChecked() && timeOfIteration * rows * rows > 3 && !confirmation()) {
        if (timeOfIteration * rows * rows > 3 && !confirmation()) {
            return;
        }
        arrayFunctions::combSort(arr, rows);
    }
    sorted = true;
    ui->deleteDup->setDisabled(!sorted);
    finding = true;
    fillTable(arr, rows);
    ui->indexesLabel->hide();
    ui->findMethodLabel->hide();
    finding = false;
}


void MainWindow::on_maxButton_pressed() {
    double max = arrayFunctions::max(arr, rows);
    ui->max->setNum(max);
    ui->max->show();
}


void MainWindow::on_minButton_pressed() {
    double min = arrayFunctions::min(arr, rows);
    ui->min->setNum(min);
    ui->min->show();
}


void MainWindow::on_avgButton_pressed() {
    double avg = arrayFunctions::avg(arr, rows);
    ui->avg->setNum(avg);
    ui->avg->show();
}


void MainWindow::on_deleteDup_clicked() {
    int newSize;
    arr = arrayFunctions::deleteDuplicates(arr, rows, &newSize);
    inited = arrayFunctions::resize(inited, rows, newSize);
    ui->size->setText(QString::number(newSize));
    finding = true;
    fillTable(arr, newSize);
    finding = false;
    ui->inputTable->setRowCount(newSize);
    rows = newSize;
    ui->indexesLabel->hide();
    ui->findMethodLabel->hide();
}

