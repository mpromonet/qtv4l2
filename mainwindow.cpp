/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(V4L2DeviceReader &v4l2reader, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene());
    connect(&v4l2reader, SIGNAL(dataReceived(QPixmap*)), this, SLOT(writePixmap(QPixmap*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeLabel(const QString & deviceName)
{
    ui->DeviceName->setText(deviceName);
}

void MainWindow::writePixmap(QPixmap * pixmap)
{
    QGraphicsScene* scene = ui->graphicsView->scene();
    scene->clear();
    scene->addPixmap(*pixmap);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
