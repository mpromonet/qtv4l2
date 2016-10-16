/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** -------------------------------------------------------------------------*/

#include <QPixmap>

#include "v4l2devicereader.h"
#include "logger.h"

#include <jpeglib.h>

unsigned long yuyv2jpeg(char* image_buffer, unsigned int width, unsigned int height, unsigned int quality)
{
    struct jpeg_error_mgr jerr;
    struct jpeg_compress_struct cinfo;
    jpeg_create_compress(&cinfo);
    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.err = jpeg_std_error(&jerr);

    unsigned char* dest = NULL;
    unsigned long  destsize = 0;
    jpeg_mem_dest(&cinfo, &dest, &destsize);
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    unsigned char bufline[cinfo.image_width * 3];
    while (cinfo.next_scanline < cinfo.image_height)
    {
        // convert line from YUYV -> YUV
        for (unsigned int i = 0; i < cinfo.image_width; i += 2)
        {
            unsigned int base = cinfo.next_scanline*cinfo.image_width * 2 ;
            bufline[i*3  ] = image_buffer[base + i*2  ];
            bufline[i*3+1] = image_buffer[base + i*2+1];
            bufline[i*3+2] = image_buffer[base + i*2+3];
            bufline[i*3+3] = image_buffer[base + i*2+2];
            bufline[i*3+4] = image_buffer[base + i*2+1];
            bufline[i*3+5] = image_buffer[base + i*2+3];
        }
        JSAMPROW row = bufline;
        jpeg_write_scanlines(&cinfo, &row, 1);
    }
    jpeg_finish_compress(&cinfo);
    if (dest != NULL)
    {
        if (destsize < width*height*2)
        {
            memcpy(image_buffer, dest, destsize);
        }
        else
        {
            LOG(WARN) << "Buffer to small size:" << width*height*2 << " " << destsize;
        }
        free(dest);
    }
    jpeg_destroy_compress(&cinfo);

    return destsize;
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
    int bufferSize = m_videoCapture->getBufferSize();

    char buffer[bufferSize];
    size_t nb = m_videoCapture->read(buffer, bufferSize );
    LOG(DEBUG) << "image size:" << nb;

    size_t size = yuyv2jpeg(buffer, width, height, 95);
    LOG(DEBUG) << "jpeg size:" << size;

    QPixmap pixmap(width, height);
    if (!pixmap.loadFromData((const uchar*)buffer, sizeof(buffer)))
    {
        LOG(WARN) << "cannot load data to QPixmap";
    }

    // signal
    emit dataReceived(&pixmap);
}
