#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void writePixmap(const QPixmap & pixmap);
    void writeLabel(const QString & deviceName);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
