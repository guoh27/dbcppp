cmake_minimum_required(VERSION 3.12)

project("dbcppp" VERSION 3.9.0)

# CONFIGURATION

set(CMAKE_CXX_STANDARD 17)
option(BUILD_DBCPPP_TOOLS "Build dbcppp utility application" OFF)
option(BUILD_DBCPPP_TESTS "Build tests" OFF)
option(BUILD_DBCPPP_EXAMPLES "Build examples" OFF)
option(BUILD_DBCPPP_SHARED "Build shared library" ON)

# DEPENDENCIES & Requirements

find_package(Boost REQUIRED CONFIG)

if(NOT Boost_FOUND)
    message(FATAL "Boost not found. Using libdbcppp boost (third-party/boost)")
endif()

# CREATE LIBRARY

file(GLOB include "include/dbcppp/*.h")
file(GLOB headers "src/*.h")
file(GLOB sources "src/*.cpp")

if(BUILD_DBCPPP_SHARED)
    add_library(${PROJECT_NAME} SHARED ${include} ${headers} ${sources})
else()
    add_library(${PROJECT_NAME} STATIC ${include} ${headers} ${sources})
endif(BUILD_DBCPPP_SHARED)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # https://en.cppreference.com/w/cpp/filesystem
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1")
        target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
    endif()
endif()

# CONFIGURE LIBRARY

target_compile_options(${PROJECT_NAME} PRIVATE -Wno-switch)

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

if (BUILD_DBCPPP_TOOLS)
    add_subdirectory(tools/dbcppp)
endif()

if (BUILD_DBCPPP_TESTS)
  add_subdirectory(tests)
endif()

if (BUILD_DBCPPP_EXAMPLES)
  add_subdirectory(examples)
endif()

# PACKAGE (useful for debugging install, use make package)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY NO)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES \\.git/ build/ ".*~$")
include(CPack)
