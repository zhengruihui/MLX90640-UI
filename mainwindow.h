#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

struct Upload
{
    unsigned short target_temperature[768];
    unsigned short self_temperature;

};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    struct Upload upload;


public slots:
    void receive();

private slots:
    void on_pushButton_clicked();

private:
    QUdpSocket *udp_socket;

private:
    Ui::MainWindow *ui;
};














#endif // MAINWINDOW_H
