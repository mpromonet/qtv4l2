/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#include <QApplication>
#include <QCommandLineParser>

#include "logger.h"
#include "V4l2Capture.h"
#include "v4l2devicereader.h"
#include "mainwindow.h"

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


        ret = application.exec();
    }
    else
    {
        LOG(ERROR) << "Cannot open device:" << deviceName.toStdString();
    }

    return ret;
}
