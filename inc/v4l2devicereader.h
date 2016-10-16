/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#ifndef V4L2DEVICEREADER_H
#define V4L2DEVICEREADER_H

#include <QObject>
#include <QSocketNotifier>

#include "V4l2Capture.h"

class V4L2DeviceReader : public QObject
{
    Q_OBJECT

public:
    V4L2DeviceReader(V4l2Capture* videoCapture, QObject *parent = 0);

signals:
    void dataReceived(QPixmap*);

public slots:
    void handleRead();

private:
    QSocketNotifier m_notifier;
    V4l2Capture*    m_videoCapture;
};

#endif // V4L2DEVICEREADER_H
