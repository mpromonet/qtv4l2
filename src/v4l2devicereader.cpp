/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#include <QByteArray>
#include <QImage>
#include <QPixmap>

#include "v4l2devicereader.h"
#include "logger.h"

#include <climits>

static int clampColor(int value)
{
    return value < 0 ? 0 : (value > 255 ? 255 : value);
}


V4L2DeviceReader::V4L2DeviceReader(V4l2Capture* videoCapture, QObject *parent)
    : QObject(parent)
    , m_notifier(videoCapture->getFd(), QSocketNotifier::Read)
    , m_videoCapture(videoCapture)
{
    connect(&m_notifier, SIGNAL(activated(int)), this, SLOT(handleRead()));
}

void V4L2DeviceReader::handleRead()
{
    int width  = m_videoCapture->getWidth();
    int height = m_videoCapture->getHeight();
    unsigned int bufferSize = m_videoCapture->getBufferSize();

    if (width <= 0 || height <= 0 || width % 2 != 0 ||
        bufferSize > static_cast<unsigned int>(INT_MAX))
    {
        LOG(ERROR) << "Invalid YUYV frame dimensions or buffer size";
        return;
    }

    QByteArray buffer(static_cast<int>(bufferSize), '\0');
    size_t nb = m_videoCapture->read(buffer.data(), buffer.size());
    LOG(DEBUG) << "image size:" << nb;

    const size_t frameSize = static_cast<size_t>(width) * height * 2;
    if (nb < frameSize || nb > static_cast<size_t>(buffer.size()))
    {
        LOG(ERROR) << "Incomplete or invalid YUYV frame:" << nb << " expected at least:" << frameSize;
        return;
    }

    QImage image(width, height, QImage::Format_RGB32);
    if (image.isNull())
    {
        LOG(ERROR) << "Cannot allocate image:" << width << "x" << height;
        return;
    }

    const unsigned char* yuyv = reinterpret_cast<const unsigned char*>(buffer.constData());
    for (int y = 0; y < height; ++y)
    {
        QRgb* pixels = reinterpret_cast<QRgb*>(image.scanLine(y));
        const unsigned char* row = yuyv + static_cast<size_t>(y) * width * 2;
        for (int x = 0; x < width; x += 2)
        {
            const int y0 = row[x * 2];
            const int u = row[x * 2 + 1] - 128;
            const int y1 = row[x * 2 + 2];
            const int v = row[x * 2 + 3] - 128;
            const int chromaU = 516 * u;
            const int red = 409 * v;
            const int green = -100 * u - 208 * v;

            const int c0 = 298 * (y0 - 16);
            const int c1 = 298 * (y1 - 16);
            pixels[x] = qRgb(clampColor((c0 + red + 128) >> 8),
                             clampColor((c0 + green + 128) >> 8),
                             clampColor((c0 + chromaU + 128) >> 8));
            pixels[x + 1] = qRgb(clampColor((c1 + red + 128) >> 8),
                                 clampColor((c1 + green + 128) >> 8),
                                 clampColor((c1 + chromaU + 128) >> 8));
        }
    }

    QPixmap pixmap = QPixmap::fromImage(image);

    // signal
    emit dataReceived(&pixmap);
}
