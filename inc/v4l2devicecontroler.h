/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#ifndef V4L2DEVICECONTROLER_H
#define V4L2DEVICECONTROLER_H

#include <QObject>

class V4L2DeviceControler : public QObject
{
    Q_OBJECT
public:
    V4L2DeviceControler(int fd, QObject *parent = 0);

signals:

public slots:
    void controlUpdated(int id, int value);

private:
    int  m_fd;
};

#endif // V4L2DEVICECONTROLER_H
