
# Implementation of stb_image and helper functions
add_library(stb_image stb_image.cpp stb_image.h stb_image_write.h stb_image_resize.h)
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
