#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <cmath>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), listModel(new QStringListModel(this)) {
    ui->setupUi(this);
    ui->listView->setModel(listModel);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_execute_clicked() {
    QString input = ui->lineEdit->text();
    bool isInt = false, isReal = false;
    qlonglong intich = input.toLongLong(&isInt);
    double realich = input.toDouble(&isReal);

    QString result;

    if (isInt) {
        int bits = 0;
        if (intich >= -128 && intich <= 127)
            result += "Подходящий тип: qint8\n";
        else
            result += "Подходящий тип: qlonglong\n";

        bits = static_cast<int>(ceil(log2(abs(intich) + 1))) + 1;
        result += QString("Бит: %1\n").arg(bits);
        result += QString("Байт: %1\n").arg((bits + 7) / 8);
    } else if (isReal) {
        result += "Подходящий тип: qreal\n";
        result += QString("Байт: %1\n").arg(sizeof(qreal));
        result += QString("Бит: %1\n").arg(sizeof(qreal) * 8);
    } else {
        result = "Неверный ввод.";
    }

    lastResult = result;
    ui->textEdit->setText(result);
    QStringList list = listModel->stringList();
    list << "Число: " + input + " - " + result.split('\n').first();
    listModel->setStringList(list);
}

void MainWindow::on_save_clicked() {
    if (lastResult.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет данных для сохранения.");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как", "", "(*.txt)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << lastResult;
            file.close();
            QMessageBox::information(this, "Успешно", "Результат сохранен в файл.");
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось открыть файл для записи.");
        }
    }
}

void MainWindow::on_about_clicked() {
    QMessageBox::information(this, "О программе",
                             "Программа вычисляет размер введённого числа в байтах и битах, а также тип C++.\n"
                             "Типы qreal, qint8, qlonglong\n"
                             "Вывод на textEdit(сверху), listView(история запросов, снизу) и в текстовый файл");
}
