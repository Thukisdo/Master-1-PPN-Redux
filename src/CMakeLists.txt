

add_library(common INTERFACE
        global.h
        type.h
        )
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(io)
add_subdirectory(image)
add_subdirectory(convolution_neural_network)
add_subdirectory(context)


# Main executable for the project
add_executable(main main.cpp)
target_link_libraries(main PRIVATE
        common image convolution_neural_network context
        spdlog::spdlog
        io
        )

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# Test executable for the project
add_executable(test testing.c)
target_link_libraries(test PRIVATE common image convolution_neural_network context)
set_target_properties(test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



