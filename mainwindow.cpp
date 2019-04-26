#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    udp_socket = new QUdpSocket;
    udp_socket->bind(QHostAddress("192.168.1.139"), 168);
    udp_socket->
    connect(udp_socket, SIGNAL(readyRead()), this, SLOT(receive()));
    on_pushButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receive()
{
    QByteArray ba;
    while(udp_socket->hasPendingDatagrams())
    {
        QByteArray temp_ba;
        temp_ba.resize(udp_socket->pendingDatagramSize());
        udp_socket->readDatagram(temp_ba.data(), temp_ba.size());
        ba.append(temp_ba);
    }
    if(ba.size() != sizeof(struct Upload))
    {
        qDebug() << "UDP丢包";
        return;
    }
    memcpy((void *)&upload, (void *)ba.data(), sizeof(struct Upload));

    for(int row=0; row<24;row++)
    {
        for(int column=0; column<32;column++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(QString("%1").arg((float)upload.target_temperature[32*row+column]/100));

            item->setBackgroundColor(QColor(((upload.target_temperature[32*row+column]/100+273)*0xffffff/340)));

            ui->tableWidget->setItem(row, 31-column, item);
        }
    }


}

void MainWindow::on_pushButton_clicked()
{
    unsigned char write_data[] = {0xaa, 0x55, 0x00, 0x06, 0x07, 0x47, 0x32, 0x38, 0xc8, 0x2d };
    this->udp_socket->writeDatagram((char *)write_data, sizeof(write_data), QHostAddress("192.168.1.79"), 167);
}



