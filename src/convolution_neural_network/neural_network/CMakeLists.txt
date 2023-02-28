add_library(neural_network STATIC
        neural_network.c neural_network.h
)

target_link_libraries(neural_network PUBLIC context)
target_include_directories(neural_network PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})



