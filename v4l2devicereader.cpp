
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


V4L2DeviceReader::V4L2DeviceReader(V4l2Capture* videoCapture, MainWindow * w, QObject *parent)
    : QObject(parent)
    , m_notifier(videoCapture->getFd(), QSocketNotifier::Read)
    , m_videoCapture(videoCapture)
    , m_mainWindow(w)
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
    std::cout << "image size:" << nb << std::endl;

    size_t size = yuyv2jpeg(buffer, width, height, 95);
    std::cout << "jpeg size:" << size << std::endl;

    QPixmap pixmap(width, height);
    bool ret = pixmap.loadFromData((const uchar*)buffer, sizeof(buffer));
    std::cout << ret << std::endl;

    m_mainWindow->writePixmap(pixmap);

    emit dataReceived();
}
