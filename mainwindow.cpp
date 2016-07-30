#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeLabel(const QString & deviceName)
{
    ui->DeviceName->setText(deviceName);
}

void MainWindow::writePixmap(const QPixmap & pixmap)
{
    QGraphicsScene* scene = new QGraphicsScene();
    scene->addPixmap(pixmap);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}
