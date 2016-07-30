/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "v4l2devicereader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(V4L2DeviceReader & v4l2reader, QWidget *parent = 0);
    ~MainWindow();

    void writeLabel(const QString & deviceName);

private slots:
    void writePixmap(QPixmap * pixmap);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
