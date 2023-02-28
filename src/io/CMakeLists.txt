

add_library(io SHARED DatasetInfo.cpp DatasetInfo.hpp Dataset.cpp Dataset.hpp)
set_target_properties(io PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
target_include_directories(io PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(io PUBLIC spdlog::spdlog)