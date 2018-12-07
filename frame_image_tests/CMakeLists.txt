# add the executable
add_executable (frame_image_test
        maintest.cpp
        frame_image_test.cpp
        ../slicer_encoder_code_sample/ImageFrame.cpp
        )
# link boost with the test
target_link_libraries(frame_image_test
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )