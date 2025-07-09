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


    auto calculateFloatingPointBits = [](double value) -> int {
        if (value == 0.0)
            return 1;

        value = fabs(value);

        double intpart;
        double fracpart = modf(value, &intpart);

        int intBits = 0;
        if (intpart > 0)
            intBits = static_cast<int>(floor(log2(intpart))) + 1;

        int fracBits = 0;
        double temp = fracpart;
        const int maxFractionBits = 52;
        while (temp != 0.0 && fracBits < maxFractionBits) {
            temp *= 2;
            temp = temp - floor(temp);
            ++fracBits;
        }

        int exponentBits = 11;
        int signBit = 1;

        return signBit + exponentBits + intBits + fracBits;
    };

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
        result += QString("Размер числа (теоретический):\nБит: %1\n").arg(bits);
        result += QString("Байт: %1\n").arg((bits + 7) / 8);

        if (intich >= -128 && intich <= 127) {
            result += QString("\nРеальный размер (qint8): %1 бит, %2 байт\n")
                          .arg(sizeof(qint8) * 8)
                          .arg(sizeof(qint8));
        } else {
            result += QString("\nРеальный размер (qlonglong): %1 бит, %2 байт\n")
                          .arg(sizeof(qlonglong) * 8)
                          .arg(sizeof(qlonglong));
        }
    } else if (isReal) {
        result += "Подходящий тип: qreal\n";

        int tBit = calculateFloatingPointBits(realich);
        int tByt = (tBit + 7) / 8;
        result += QString("Размер числа (теоретический):\nБит: %1\nБайт: %2\n")
                      .arg(tBit)
                      .arg(tByt);

        result += QString("\nРеальный размер (qreal): %1 бит, %2 байт\n")
                      .arg(sizeof(qreal) * 8)
                      .arg(sizeof(qreal));
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
