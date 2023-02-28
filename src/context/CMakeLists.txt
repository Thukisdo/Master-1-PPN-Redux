
find_package(libconfig REQUIRED)

add_library(context STATIC
        context.c context.h
)

target_include_directories(context PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(context PRIVATE ${LIBCONFIG_INCLUDE_DIRS})
target_link_libraries(context PUBLIC common ${LIBCONFIG_LIBRARIES})



