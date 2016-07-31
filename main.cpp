/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#include <QApplication>
#include <QCommandLineParser>

#include <stropts.h>

#include "logger.h"
#include "V4l2Capture.h"
#include "v4l2devicereader.h"
#include "mainwindow.h"

unsigned int add_ctrl(int fd, unsigned int i, MainWindow & w)
{
    unsigned int ret=0;
    struct v4l2_queryctrl   qctrl;
    memset(&qctrl,0,sizeof(qctrl));
    qctrl.id = i;
    if (0 == ioctl(fd,VIDIOC_QUERYCTRL,&qctrl))
    {
        if (!(qctrl.flags & V4L2_CTRL_FLAG_DISABLED))
        {
            struct v4l2_control control;
            memset(&control,0,sizeof(control));
            control.id = qctrl.id;
            if (0 == ioctl(fd,VIDIOC_G_CTRL,&control))
            {
                if ( (qctrl.type == V4L2_CTRL_TYPE_MENU)
#ifdef V4L2_CTRL_TYPE_INTEGER_MENU
                    || (qctrl.type == V4L2_CTRL_TYPE_INTEGER_MENU)
#endif
                   )
                {
                    struct v4l2_querymenu querymenu;
                    memset(&querymenu,0,sizeof(querymenu));
                    querymenu.id = qctrl.id;

                    for (querymenu.index = 0; querymenu.index <= qctrl.maximum; querymenu.index++)
                    {
                        if (0 == ioctl(fd,VIDIOC_QUERYMENU,&querymenu))
                        {
                            QString label;
                            if (qctrl.type == V4L2_CTRL_TYPE_MENU)
                            {
                                label = (const char*)querymenu.name;
                            }
#ifdef V4L2_CTRL_TYPE_INTEGER_MENU
                            else if (qctrl.type == V4L2_CTRL_TYPE_INTEGER_MENU)
                            {
                                label = (int)querymenu.value;
                            }
#endif
                        }
                    }
                }
                else
                {
                    w.addSliderControl((const char*)qctrl.name, control.value, qctrl.minimum, qctrl.maximum);
                }
            }
        }
        ret = qctrl.id;
    }
    return ret;
}

int main(int argc, char *argv[])
{
    // create command line parser
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption deviceOpt("d", "device", "V4L2 device name", "/dev/video0");
    parser.addOption(deviceOpt);
    QCommandLineOption widthOpt("W", "width", "capture width", "0");
    parser.addOption(widthOpt);
    QCommandLineOption heightOpt("H", "height", "capture height", "0");
    parser.addOption(heightOpt);

    // parse command line
    QApplication application(argc, argv);
    parser.process(application);

    QString deviceName(parser.value(deviceOpt));

    int ret = -1;
    // create capture interface
    V4L2DeviceParameters param(deviceName.toStdString().c_str()
                               , V4L2_PIX_FMT_YUYV
                               , parser.value(widthOpt).toInt()
                               , parser.value(heightOpt).toInt()
                               , 0, 255);
    V4l2Capture* videoCapture = V4l2DeviceFactory::CreateVideoCapure(param, V4l2DeviceFactory::IOTYPE_MMAP);
    if (videoCapture)
    {
        V4L2DeviceReader reader(videoCapture);

        // create window
        MainWindow w;
        w.writeDeviceName(deviceName);
        w.writeCaptureSize(videoCapture->getWidth(), videoCapture->getHeight());
        w.show();

        // connect reader signal to window slot
        QObject::connect(&reader, SIGNAL(dataReceived(QPixmap*)), &w, SLOT(writePixmap(QPixmap*)));

        // add controls
        int fd = videoCapture->getFd();
        for (unsigned int i = V4L2_CID_BASE; i<V4L2_CID_LASTP1; add_ctrl(fd,i,w), i++);
        for (unsigned int i = V4L2_CID_LASTP1+1; i != 0 ; i=add_ctrl(fd,i|V4L2_CTRL_FLAG_NEXT_CTRL,w));

        ret = application.exec();
    }
    else
    {
        LOG(ERROR) << "Cannot open device:" << deviceName.toStdString();
    }

    return ret;
}
