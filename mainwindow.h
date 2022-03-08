#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog> //либа для  диалоговых окон работы с файлом
#include <QMessageBox> // либа для окна с ошибкой
#include <QCloseEvent>//событие выхода
//#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void closeEvent(QCloseEvent *event); //обработка сигнала закрытия формы

    void on_action_newfile_triggered();

    bool on_action_savefileas_triggered();

    void on_action_openfile_triggered();

    bool on_action_savefile_triggered();

    void on_action_zoomin_triggered();

    void on_action_zoomout_triggered();

    void on_action_reset_triggered();

    void on_action_newwindow_triggered();

    void on_action_exit_triggered();

private:
    Ui::MainWindow *ui;
    QString directory_path = "C:\\";
};
#endif // MAINWINDOW_H
