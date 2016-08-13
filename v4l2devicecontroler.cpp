/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#include <stropts.h>
#include <errno.h>
#include <linux/videodev2.h>

#include "logger.h"
#include "v4l2devicecontroler.h"

V4L2DeviceControler::V4L2DeviceControler(int fd, QObject *parent) : QObject(parent), m_fd(fd)
{
}

void V4L2DeviceControler::controlUpdated(int id, int value)
{
    struct v4l2_control control;
    memset(&control,0,sizeof(control));
    control.id = id;
    control.value = value;
    if (ioctl(m_fd,VIDIOC_S_CTRL,&control) != 0)
    {
        LOG(WARN) << "ioctl failed errno:" << errno;
    }
    else
    {
        LOG(DEBUG) << "ioctl OK";
    }
}
