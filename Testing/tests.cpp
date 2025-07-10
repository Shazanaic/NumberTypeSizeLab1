#include <QtTest>
#include <QMessageBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include "mainwindow.h"

class tests : public QObject {
    Q_OBJECT

private slots:
    void testIntegerInput();
    void testRealInput();
    void testInvalidInput();
    void testAboutButton();
};

void tests::testIntegerInput() {
    MainWindow w;
    auto* lineEdit = w.findChild<QLineEdit*>("lineEdit");
    auto* textEdit = w.findChild<QTextEdit*>("textEdit");
    auto* button = w.findChild<QPushButton*>("execute");

    QVERIFY(lineEdit);
    QVERIFY(textEdit);
    QVERIFY(button);

    lineEdit->setText("123");
    w.show();
    QTest::mouseClick(button, Qt::LeftButton);

    const QString output = textEdit->toPlainText();
    QVERIFY(output.contains("Подходящий тип"));
    QVERIFY(output.contains("Бит"));
    QVERIFY(output.contains("Байт"));
}

void tests::testRealInput() {
    MainWindow w;
    auto* lineEdit = w.findChild<QLineEdit*>("lineEdit");
    auto* textEdit = w.findChild<QTextEdit*>("textEdit");
    auto* button = w.findChild<QPushButton*>("execute");

    QVERIFY(lineEdit);
    QVERIFY(textEdit);
    QVERIFY(button);

    lineEdit->setText("3.14");
    w.show();
    QTest::mouseClick(button, Qt::LeftButton);

    const QString output = textEdit->toPlainText();
    QVERIFY(output.contains("qreal"));
    QVERIFY(output.contains("Бит"));
    QVERIFY(output.contains("Байт"));
}

void tests::testInvalidInput() {
    MainWindow w;
    auto* lineEdit = w.findChild<QLineEdit*>("lineEdit");
    auto* textEdit = w.findChild<QTextEdit*>("textEdit");
    auto* button = w.findChild<QPushButton*>("execute");

    QVERIFY(lineEdit);
    QVERIFY(textEdit);
    QVERIFY(button);

    lineEdit->setText("bebra13");
    w.show();
    QTest::mouseClick(button, Qt::LeftButton);

    QCOMPARE(textEdit->toPlainText(), QString("Неверный ввод."));
}

void tests::testAboutButton() {
    MainWindow w;
    auto* button = w.findChild<QPushButton*>("about");
    QVERIFY(button);
    QTimer::singleShot(0, [&]() {
        QWidgetList widgets = QApplication::topLevelWidgets();
        for (QWidget* widget : widgets) {
            if (auto* box = qobject_cast<QMessageBox*>(widget)) {
                QCOMPARE(box->windowTitle(), QString("О программе"));
                QVERIFY(box->text().contains("Программа вычисляет размер"));
                box->close();
            }
        }
    });

    w.show();
    QTest::mouseClick(button, Qt::LeftButton);
}

QTEST_MAIN(tests)
#include "tests.moc"
