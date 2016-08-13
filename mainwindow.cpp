/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene());
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->widget()->setLayout(ui->propertiesLayout);
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
    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setProperty("id",id);
    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setSingleStep(step);
    slider->setValue(value);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(sliderChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(new QLabel(ctrl));
    layout->addWidget(slider);

    ui->propertiesLayout->addLayout(layout);
}

void MainWindow::sliderChanged(int value)
{
    // retreive control id
    QSlider* slider = (QSlider*)sender();
    int id = slider->property("id").toInt();

    // signal update
    emit controlUpdated(id, value);
}

void MainWindow::addCombobox(const QString & ctrl, const QList< QPair<int, QString> > & texts, int id, int value)
{
    QComboBox* comboBox = new QComboBox();
    comboBox->setProperty("id",id);
    QList< QPair<int, QString> >::const_iterator it;
    for (it = texts.begin(); it != texts.end(); ++it)
    {
        comboBox->addItem(it->second,it->first);
    }
    int currentIdx = comboBox->findData(value);
    if (currentIdx != -1)
    {
        comboBox->setCurrentIndex(currentIdx);
    }
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxChanged(int)));

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(new QLabel(ctrl));
    layout->addWidget(comboBox);

    ui->propertiesLayout->addLayout(layout);
}

void MainWindow::comboBoxChanged(int idx)
{
    // retreive control id
    QComboBox* comboBox = (QComboBox*)sender();
    int id = comboBox->property("id").toInt();

    int value = comboBox->itemData(idx).toInt();

    // signal update
    emit controlUpdated(id, value);
}

