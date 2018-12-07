#include "ImageFrame.h"
#include <stddef.h>

CImageFrame::CImageFrame() :
        _width(0),
        _height(0),
        _yPlane(NULL),
        _uPlane(NULL),
        _vPlane(NULL)
{
}

CImageFrame::CImageFrame(uint8_t* yuvPlanar, long width, long height) :
    _width(0),
    _height(0),
    _yPlane(NULL),
    _uPlane(NULL),
    _vPlane(NULL)
{
    const long uPLANEOFFSET = 2;

    if (yuvPlanar)
    {
        _width = width;
        _height = height;
        _yPlane = yuvPlanar;
        _uPlane = yuvPlanar + (width * height);
        _vPlane = yuvPlanar + (uPLANEOFFSET * width * height);
    }
}

// Blend the Top and bottom images using the matte
// return zero on success and non-zero on failure
int CImageFrame::Blend(const CImageFrame& top, const CImageFrame& bottom, const CImageFrame& matte)
{
    if (_width != top._width || _width != bottom._width || _width != matte._width ||
        _height != top._height || _height != bottom._height || _height != matte._height)
    {
        return 1;
    }

    // Could be a double for loop if added row bytes for non-contiguous images
    for (long x = 0; x < _width * _height; x++)
    {
        _yPlane[x] = (uint8_t) (((uint16_t) top._yPlane[x]) * matte._yPlane[x] / 255 +
                                 ((uint16_t) bottom._yPlane[x]) * (255 - matte._yPlane[x]) / 255);
        _uPlane[x] = (uint8_t) (((uint16_t) top._uPlane[x]) * matte._yPlane[x] / 255 +
                                 ((uint16_t) bottom._uPlane[x]) * (255 - matte._yPlane[x]) / 255);
        _vPlane[x] = (uint8_t) (((uint16_t) top._vPlane[x]) * matte._yPlane[x] / 255 +
                                 ((uint16_t) bottom._vPlane[x]) * (255 - matte._yPlane[x]) / 255);
    }

    return 0;
}

// Blend the Top and bottom images using the matte
// return zero on success and non-zero on failure
int CImageFrame::BlendFixedPoint(const CImageFrame& top, const CImageFrame& bottom, const CImageFrame& matte)
{
    if (_width != top._width || _width != bottom._width || _width != matte._width ||
        _height != top._height || _height != bottom._height || _height != matte._height)
    {
        return 1;
    }

    // Could be a double for loop if added row bytes for non-contiguous images
    for (long x = 0; x < _width * _height; x++)
    {
        uint32_t topFixedPercent = (((uint32_t) matte._yPlane[x]) << 16) / 0xFF;
        uint32_t botFixedPercent = ((uint32_t) (255 - matte._yPlane[x]) << 16) / 0xFF;

        _yPlane[x] = (uint8_t) ((((uint32_t) top._yPlane[x]) * topFixedPercent +
                                 ((uint32_t) bottom._yPlane[x]) * botFixedPercent) >> 16);
        _uPlane[x] = (uint8_t) ((((uint32_t) top._uPlane[x]) * topFixedPercent +
                                 ((uint32_t) bottom._uPlane[x]) * botFixedPercent) >> 16);
        _vPlane[x] = (uint8_t) ((((uint32_t) top._vPlane[x]) * topFixedPercent +
                                 ((uint32_t) bottom._vPlane[x]) * botFixedPercent) >> 16);
    }

    return 0;
}

uint8_t* CImageFrame::GetYuvPlanar() const
{
    return _yPlane;
}

long CImageFrame::GetWidth() const
{
    return _width;
}

long CImageFrame::GetHeight() const
{
    return _height;
}