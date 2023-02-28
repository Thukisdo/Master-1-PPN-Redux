
find_package(OpenMP REQUIRED)

add_library(io STATIC
        DatasetInfo.cpp DatasetInfo.hpp
        Dataset.cpp Dataset.hpp
        Image.cpp Image.hpp
)
target_link_libraries(io PRIVATE spdlog::spdlog stb_image OpenMP::OpenMP_CXX)
target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(io PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
)