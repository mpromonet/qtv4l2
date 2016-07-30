#include <QApplication>
#include <QCommandLineParser>
#include <QGraphicsScene>

#include "V4l2Capture.h"
#include "logger.h"
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

    // parse command line
    QApplication application(argc, argv);
    parser.process(application);

    QString deviceName(parser.value(deviceOpt));

    // create window
    MainWindow w;
    w.writeLabel(deviceName);
    w.show();

    int ret = -1;
    V4L2DeviceParameters param(deviceName.toStdString().c_str(), V4L2_PIX_FMT_YUYV, 0, 0 , 0, 255);
    V4l2Capture* videoCapture = V4l2DeviceFactory::CreateVideoCapure(param, V4l2DeviceFactory::IOTYPE_MMAP);
    if (videoCapture)
    {
        V4L2DeviceReader reader(videoCapture, &w);
        ret = application.exec();
    }
    else
    {
        std::cout << "Cannot open device:" << deviceName.toStdString() << std::endl;
    }

    return ret;
}
