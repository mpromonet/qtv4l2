#ifndef V4L2DEVICEREADER_H
#define V4L2DEVICEREADER_H

#include <QObject>
#include <QSocketNotifier>

#include "V4l2Capture.h"
#include "mainwindow.h"

class V4L2DeviceReader : public QObject
{
    Q_OBJECT
public:
    explicit V4L2DeviceReader(QObject *parent = 0);
    V4L2DeviceReader(V4l2Capture* videoCapture, MainWindow * w, QObject *parent = 0);

signals:
    void dataReceived();

public slots:
    void handleRead();

private:
    QSocketNotifier m_notifier;
    V4l2Capture*    m_videoCapture;
    MainWindow*     m_mainWindow;
};

#endif // V4L2DEVICEREADER_H
