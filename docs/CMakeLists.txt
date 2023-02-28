
find_package(Doxygen COMPONENTS dot)

if (DOXYGEN_FOUND)
  message(STATUS "Doxygen found")

  # We use the doxygen-awesome-css stylesheets to make the doxygen output look
  # modern and nice.
  set(DOXYGEN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/docs)
  set(DOXYGEN_INPUT_LIST "${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/README.md")
  set(DOXYGEN_MAINPAGE_MD ${PROJECT_SOURCE_DIR}/README.md)
  set(DOXYGEN_EXTRA_HTML_STYLESHEET
          "${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome.css
           ${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome-sidebar-only.css")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${PROJECT_BINARY_DIR}/docs/Doxyfile @ONLY)
  add_custom_target(doxygen
    BYPRODUCTS ${PROJECT_BINARY_DIR}/docs/html/index.html
    COMMAND Doxygen::doxygen ${PROJECT_BINARY_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${PROJECT_BINARY_DIR}/docs/Doxyfile
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
else()
  message(WARNING "Doxygen not found, not building documentation")
endif()
