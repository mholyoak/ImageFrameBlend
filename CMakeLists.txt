cmake_minimum_required(VERSION 3.12)
project(slicersample)

set(CMAKE_CXX_STANDARD 98)

add_executable(codesample
        slicer_encoder_code_sample/main.cpp
        slicer_encoder_code_sample/ImageFrame.cpp slicer_encoder_code_sample/ImageFrame.h)

add_subdirectory(frame_image_tests)