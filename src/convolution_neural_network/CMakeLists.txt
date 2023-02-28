

add_subdirectory(convolution_layer)
add_subdirectory(neural_network)

add_library(convolution_neural_network STATIC
        store.c store.h
        evaluation.c evaluation.h
        training.c training.h
)
target_include_directories(convolution_neural_network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(convolution_neural_network PUBLIC convolution_layer neural_network image context)


