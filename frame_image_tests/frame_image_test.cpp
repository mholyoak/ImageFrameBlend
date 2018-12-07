#include <boost/test/unit_test.hpp>
#include "../slicer_encoder_code_sample/ImageFrame.h"
#include <vector>
#include <iostream>

#define BLEND_MATTE_TEST_CASE(name) BOOST_FIXTURE_TEST_CASE(blend_matte_test_##name, BlendMatteTestFixture)

struct BlendMatteTestFixture
{
    static const long       imageWidth = 2;
    static const long       imageHeight = 2;
    static const long       numPlanes = 3;
    std::vector<uint8_t>    topYuvBuffer;
    std::vector<uint8_t>    bottomYuvBuffer;
    CImageFrame             topImage;
    CImageFrame             bottomImage;

    BlendMatteTestFixture()
    {
        uint8_t topYuvBufferAry[] = {0xFF, 0xFF, 0xFF, 0xFF,
                                     0xF0, 0xF0, 0xF0, 0xF0,
                                     0xF1, 0xF1, 0xF1, 0xF1};
        topYuvBuffer = std::vector<uint8_t> (std::begin(topYuvBufferAry), std::end(topYuvBufferAry));
        topImage = CImageFrame(topYuvBuffer.data(), imageWidth, imageHeight);

        uint8_t bottomYuvBufferAry[] = {0xDF, 0xDF, 0xDF, 0xDF,
                                        0xEC, 0xEC, 0xEC, 0xEC,
                                        0xEF, 0xEF, 0xEF, 0xEF};
        bottomYuvBuffer = std::vector<uint8_t> (std::begin(bottomYuvBufferAry), std::end(bottomYuvBufferAry));
        bottomImage = CImageFrame(bottomYuvBuffer.data(), imageWidth, imageHeight);
    }
};


BLEND_MATTE_TEST_CASE(ErrorWithDifferentImageSizes)
{
    std::cout << "Testing Blend method with invalid parameters." << std::endl;

    uint8_t zeroYuvBuffer[] = {0, 0, 0, 0,
                               0, 0, 0, 0,
                               0, 0, 0, 0};
    CImageFrame blendImage(zeroYuvBuffer, 2, 2);

    uint8_t zeroYuvBufferSource[] = {0, 0, 0, 0,
                                     0, 0, 0, 0,
                                     0, 0, 0, 0};
    CImageFrame zeroImage2x2(zeroYuvBufferSource, 2, 2);
    CImageFrame zeroImage1x2(zeroYuvBufferSource, 1, 2);

    int result;
    result = blendImage.Blend(zeroImage1x2, zeroImage2x2, zeroImage2x2);
    BOOST_CHECK_NE(result, 0);

    result = blendImage.Blend(zeroImage2x2, zeroImage1x2, zeroImage2x2);
    BOOST_CHECK_NE(result, 0);

    result = blendImage.Blend(zeroImage2x2, zeroImage2x2, zeroImage1x2);
    BOOST_CHECK_NE(result, 0);

    CImageFrame zeroImage2x1(zeroYuvBufferSource, 2, 1);

    result = blendImage.Blend(zeroImage2x1, zeroImage2x2, zeroImage2x2);
    BOOST_CHECK_NE(result, 0);

    result = blendImage.Blend(zeroImage2x2, zeroImage2x1, zeroImage2x2);
    BOOST_CHECK_NE(result, 0);

    result = blendImage.Blend(zeroImage2x2, zeroImage2x2, zeroImage2x1);
    BOOST_CHECK_NE(result, 0);
}

BLEND_MATTE_TEST_CASE(BlendTopOnly)
{
    std::cout << "Testing Blend method 100% top image." << std::endl;

    uint8_t blendYuvBuffer[] = {0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00};
    CImageFrame blendImage(blendYuvBuffer, imageWidth, imageHeight);

    uint8_t matteYuvBuffer[] = {0xFF, 0xFF, 0xFF, 0xFF,
                               0x00, 0x00, 0x00, 0x00,
                               0x00, 0x00, 0x00, 0x00};
    CImageFrame matteImage(matteYuvBuffer, imageWidth, imageHeight);

    // Blend top and bottom image using 100% of top image
    int result = blendImage.Blend(topImage, bottomImage, matteImage);
    BOOST_CHECK_EQUAL(result, 0);

    // The Blended image should match the top image
    BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(blendYuvBuffer), std::end(blendYuvBuffer),
                                  std::begin(topYuvBuffer), std::end(topYuvBuffer));
}

BLEND_MATTE_TEST_CASE(BlendBottomOnly)
{
    std::cout << "Testing Blend method 100% bottom image." << std::endl;

    uint8_t blendYuvBuffer[] = {0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00};
    CImageFrame blendImage(blendYuvBuffer, imageWidth, imageHeight);

    uint8_t matteYuvBuffer[] = {0x00, 0x00, 0x00, 0x00,
                               0xF0, 0xF0, 0xF0, 0xF0,
                               0xF0, 0xF0, 0xF0, 0xF0};
    CImageFrame matteImage(matteYuvBuffer, imageWidth, imageHeight);

    // Blend top and bottom image using 100% of bottom image
    int result = blendImage.Blend(topImage, bottomImage, matteImage);
    BOOST_CHECK_EQUAL(result, 0);

    // The Blended image should match the bottom image
    BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(blendYuvBuffer), std::end(blendYuvBuffer),
                                  std::begin(bottomYuvBuffer), std::end(bottomYuvBuffer));
}

BLEND_MATTE_TEST_CASE(BlendBoth)
{
    std::cout << "Testing Blend method with about 75% top image and 25% bottom image." << std::endl;

    uint8_t blendYuvBuffer[] = {0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00};
    CImageFrame blendImage(blendYuvBuffer, imageWidth, imageHeight);

    // Blend 75% top so we are sure that the percent of top and bottom images is correct
    uint8_t matteYuvBuffer[] = {0xC0, 0xC0, 0xC0, 0xC0,
                               0xF5, 0xF5, 0xF5, 0xF5,
                               0xF6, 0xF6, 0xF6, 0xF6};
    CImageFrame matteImage(matteYuvBuffer, imageWidth, imageHeight);

    // Blend images with about 75% top image 25% bottom
    int result = blendImage.Blend(topImage, bottomImage, matteImage);
    BOOST_CHECK_EQUAL(result, 0);

    uint8_t expectedYuvBuffer[] = {0xF7, 0xF7, 0xF7, 0xF7,
                                   0xEE, 0xEE, 0xEE, 0xEE,
                                   0xF0, 0xF0, 0xF0, 0xF0};

    // The Blended image should match the bottom image
    BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(blendYuvBuffer), std::end(blendYuvBuffer),
                                  std::begin(expectedYuvBuffer), std::end(expectedYuvBuffer));
}

BLEND_MATTE_TEST_CASE(BlendBothFixedPoint)
{
    std::cout << "Testing Blend Fixed Point method with about 75% top image and 25% bottom image." << std::endl;

    uint8_t blendYuvBuffer[] = {0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00,
                                0x00, 0x00, 0x00, 0x00};
    CImageFrame blendImage(blendYuvBuffer, imageWidth, imageHeight);

    // Blend 75% top so we are sure that the percent of top and bottom images is correct
    uint8_t matteYuvBuffer[] = {0xC0, 0xC0, 0xC0, 0xC0,
                                0xF5, 0xF5, 0xF5, 0xF5,
                                0xF6, 0xF6, 0xF6, 0xF6};
    CImageFrame matteImage(matteYuvBuffer, imageWidth, imageHeight);
    int result = blendImage.BlendFixedPoint(topImage, bottomImage, matteImage);
    BOOST_CHECK_EQUAL(result, 0);

    uint8_t expectedYuvBuffer[] = {0xF7, 0xF7, 0xF7, 0xF7,
                                   0xEF, 0xEF, 0xEF, 0xEF,
                                   0xF0, 0xF0, 0xF0, 0xF0};

    // The Blended image should match the bottom image
    BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(blendYuvBuffer), std::end(blendYuvBuffer),
                                  std::begin(expectedYuvBuffer), std::end(expectedYuvBuffer));
}

BLEND_MATTE_TEST_CASE(BlendSpeedTest)
{
    std::cout << "Testing that Blend fixed point method over 40% faster than Blend int method." << std::endl;

    const long width8K = 7680;
    const long height8K = 4320;
    // Don't care what's in the buffer just calculating processing speed
    std::vector<uint8_t> blendImageBuffer(width8K * height8K * numPlanes);
    std::vector<uint8_t> sourceImageBuffer(width8K * height8K * numPlanes);

    CImageFrame blendImage(blendImageBuffer.data(), width8K, height8K);
    CImageFrame sourceImage(sourceImageBuffer.data(), width8K, height8K);

    std::clock_t start;
    double blendDuration;
    double blendFixedDuration;

    // Measure Blend speed for int blend
    start = std::clock();
    blendImage.Blend(sourceImage, sourceImage, sourceImage);
    blendDuration = std::clock() - start;

    // Measure Blend speed for fixed point blend
    start = std::clock();
    blendImage.BlendFixedPoint(sourceImage, sourceImage, sourceImage);
    blendFixedDuration = std::clock() - start;

    // Verify that fixed point is faster
    BOOST_CHECK_GT(blendDuration, blendFixedDuration);
    // verify that 40% faster
    double percentFaster = blendFixedDuration / blendDuration;
    std::cout << "\tPercent Faster: " << std::to_string(percentFaster * 100) << "%" << std::endl;
    BOOST_CHECK_GT(percentFaster, 0.40);
}