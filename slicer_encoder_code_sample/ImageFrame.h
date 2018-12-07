#ifndef IMAGEFRAME_H
#define IMAGEFRAME_H

#include <stdint.h>

class CImageFrame
{
public:
    CImageFrame();
    CImageFrame(uint8_t* yuvPlanar, long width, long height);

    int         Blend(const CImageFrame& top, const CImageFrame& bottom, const CImageFrame& mask);
    int         BlendFixedPoint(const CImageFrame& top, const CImageFrame& bottom, const CImageFrame& matte);

    uint8_t*    GetYuvPlanar() const;
    long        GetWidth() const;
    long        GetHeight() const;

private:
    long     _width;
    long     _height;
    uint8_t* _yPlane;
    uint8_t* _uPlane;
    uint8_t* _vPlane;
};

#endif //IMAGEFRAME_H
