#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringListModel>
#include <QFileDialog>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_execute_clicked();
    void on_save_clicked();
    void on_about_clicked();

private:
    Ui::MainWindow *ui;
    QString lastResult;
    QStringListModel *listModel;
};

#endif // MAINWINDOW_H
