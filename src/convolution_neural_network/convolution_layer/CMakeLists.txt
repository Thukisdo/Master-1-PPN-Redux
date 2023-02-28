
add_library(convolution_layer STATIC
        convolution_layer.c convolution_layer.h
)

target_include_directories(convolution_layer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



