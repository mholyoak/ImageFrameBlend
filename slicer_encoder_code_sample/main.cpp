//
//  main.c
//  slicer_encoder_code_sample
//
//  Created by Grady Player on 11/30/18.
//  Copyright © 2018 Uplynk. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "ImageFrame.h"

#define UL_UNIVERSAL_BUFFER_SIZE (640 * 480 * 3)

/**
 
 This is a coding test, the object of the test is to populate one function:
 uint8_t * ApplyMatte(uint8_t * top, uint8_t * bottom, uint8_t * matte )
 
 Foreground and background images will be represented as 8-bit YUV Planar images 480 rows each being 640 bytes wide
 memory will be returned as a malloc'd buffer of the size UL_UNIVERSAL_BUFFER_SIZE.
 
 matte images are the same format as FG and BG images, but only the Y channel is important,
 for our purposes other channels should be considered to contain garbage data (don't rely on them being 0x00 or 0xff).
 
 Areas in the matte that are completely white (0xff) will display in the output buffer as being
 the value of the corresponding byte of the top image.
 
 Areas in the matte that are completely black (0x00) will display in the output buffer as being
 the value of the corresponding byte of the bottom image.
 
 Areas in the matte that are a gray value will represent a weighted average of the top and
 bottom images (closer to white will be more like the top image, closer to black will be more like
 the bottom image)
 
 to check your output you may use any tool or library that you like, you may draw them to a gui,
 you may check the output with a third party tool such as https://www.macupdate.com/app/mac/28630/glyuvplay ,
 http://www.yuvtoolkit.com, or other tool
 
 for dev environment, please feel free to use whatever tool you are comfortable with,
 we are providing an xcode project and a Makefile.
 
 */

uint8_t * ApplyMatte(uint8_t * top, uint8_t * bottom, uint8_t * matte )
{
    uint8_t * returnBuffer = (uint8_t*) malloc(UL_UNIVERSAL_BUFFER_SIZE);
    //your briliant code here
    const long imageWidth = 640;
    const long imageHeight = 480;

    CImageFrame blendedImage(returnBuffer, imageWidth, imageHeight);
    CImageFrame topImage(top, imageWidth, imageHeight);
    CImageFrame bottomImage(bottom, imageWidth, imageHeight);
    CImageFrame matteImage(matte, imageWidth, imageHeight);

    blendedImage.BlendFixedPoint(topImage, bottomImage, matteImage);

    return blendedImage.GetYuvPlanar();
}

void test(const char * topFileName, const char * bottomFileName, const char * matteFileName, const char * outFileName )
{
    printf("Testing:\n\ttop: %s \n\tbottom: %s \n\tmatte: %s\n\n",topFileName, bottomFileName, matteFileName);
    
    FILE * topFileStream = fopen(topFileName, "r");
    assert(topFileStream);
    uint8_t * topFileBuffer = (uint8_t*) malloc(UL_UNIVERSAL_BUFFER_SIZE);
    size_t result = fread(topFileBuffer, UL_UNIVERSAL_BUFFER_SIZE, 1, topFileStream);
    assert(result == 1);

    FILE * bottomFileStream = fopen(bottomFileName, "r");
    assert(bottomFileStream);
    uint8_t * bottomFileBuffer = (uint8_t*) malloc(UL_UNIVERSAL_BUFFER_SIZE);
    result = fread(bottomFileBuffer, UL_UNIVERSAL_BUFFER_SIZE, 1, bottomFileStream);
    assert(result == 1);

    FILE * matteFileStream = fopen(matteFileName, "r");
    assert(matteFileStream);
    uint8_t * matteFileBuffer = (uint8_t*) malloc(UL_UNIVERSAL_BUFFER_SIZE);
    result = fread(matteFileBuffer, UL_UNIVERSAL_BUFFER_SIZE, 1, matteFileStream);
    assert(result == 1);

    FILE * outFileStream = fopen(outFileName, "w+");
    assert(outFileStream);
    uint8_t * outFileBuffer = ApplyMatte(topFileBuffer, bottomFileBuffer, matteFileBuffer);
    result = fwrite(outFileBuffer,UL_UNIVERSAL_BUFFER_SIZE,1,outFileStream);
    assert(result == 1);

    fclose(topFileStream);
    fclose(bottomFileStream);
    fclose(matteFileStream);
    fclose(outFileStream);
    
    free(topFileBuffer);
    free(bottomFileBuffer);
    free(matteFileBuffer);
    free(outFileBuffer);
}


#include <unistd.h>

int main(int argc, const char * argv[]) {
    // insert code here...
#ifdef XCODE
    chdir ( "../..");
#endif
    test("art/rear_window.yuv","bg/1.yuv","art/rear_window_mask.yuv","test1.yuv");
    test("art/rear_window.yuv","bg/2.yuv","art/rear_window_mask.yuv","test2.yuv");
    test("art/rear_window.yuv","bg/1.yuv","art/mask2.yuv","test3.yuv");
    test("art/rear_window.yuv","bg/2.yuv","art/mask2.yuv","test4.yuv");
    return 0;
}
