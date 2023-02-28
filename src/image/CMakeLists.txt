
add_library(image STATIC
        dataset_manager.c dataset_manager.h
        image_io.c image_io.h
)
target_include_directories(image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(image PUBLIC spng convolution_layer common)



