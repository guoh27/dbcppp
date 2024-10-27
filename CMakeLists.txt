cmake_minimum_required(VERSION 3.12)

project("dbcppp" VERSION 3.8.0)

# CONFIGURATION

set(CMAKE_CXX_STANDARD 17)
option(build_tools "Build dbcppp utility application" OFF)

option(build_tests "Build tests" OFF)
option(build_examples "Build examples" OFF)

# DEPENDENCIES & Requirements

find_package(Boost REQUIRED CONFIG)

if(NOT Boost_FOUND)
    message(FATAL "Boost not found. Using libdbcppp boost (third-party/boost)")
endif()

# CREATE LIBRARY

file(GLOB include "include/dbcppp/*.h")
file(GLOB headers "src/*.h")
file(GLOB sources "src/*.cpp")

add_library(${PROJECT_NAME} ${include} ${headers} ${sources})


# CONFIGURE LIBRARY

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${include}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/dbcppp>
    $<INSTALL_INTERFACE:include/dbcppp>
    include/
)

# INSTALL LIBRARY

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/dbcppp
)


# ADDITIONAL: Tools, Tests & Examples

if (build_tools)
    add_subdirectory(third-party/cxxopts)
    add_subdirectory(tools/dbcppp)
endif()

if (build_tests)
  add_subdirectory(tests)
endif()

if (build_examples)
  add_subdirectory(examples)
endif()

# PACKAGE (useful for debugging install, use make package)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY NO)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES \\.git/ build/ ".*~$")
include(CPack)
