/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void writeDeviceName(const QString & deviceName);
    void writeCaptureSize(int width, int height);
    void addSliderControl(const QString & ctrl, int min, int max, int step, int id, int value);
    void addCombobox(const QString & ctrl, const QList< QPair<int, QString> > & texts, int id, int value);

signals:
    void controlUpdated(int, int);

private slots:
    void writePixmap(QPixmap * pixmap);
    void sliderChanged(int value);
    void comboBoxChanged(int value);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
