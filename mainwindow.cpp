/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeDeviceName(const QString & deviceName)
{
    ui->DeviceName->setText(deviceName);
}

void MainWindow::writeCaptureSize(int width, int height)
{
    QString geometry;
    geometry.sprintf("%dx%d",width,height);
    ui->Geometry->setText(geometry);
}


void MainWindow::writePixmap(QPixmap * pixmap)
{
    QGraphicsScene* scene = ui->graphicsView->scene();
    scene->clear();
    scene->addPixmap(*pixmap);
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::addSliderControl(const QString & ctrl, int min, int max, int step, int id, int value)
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(new QLabel(ctrl));
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setProperty("id",id);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setSingleStep(step);
    slider->setValue(value);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderUpdated(int)));
    layout->addWidget(slider);
    ui->propertiesLayout->addLayout(layout);
}

void MainWindow::sliderUpdated(int value)
{
    // retreive control id
    QSlider* slider = (QSlider*)sender();
    int id = slider->property("id").toInt();

    // signal update
    emit controlUpdated(id, value);
}
