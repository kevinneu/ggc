cmake_minimum_required(VERSION 2.6)
project(ggc)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
  -g
  -Wall
  -MD
  -Wextra
  -Werror
  -Wconversion
  -Wno-unused-parameter
  -Woverloaded-virtual
  -Wpointer-arith
  -Wshadow
  -Wwrite-strings
  -march=native
  -rdynamic
  )


if(CMKAE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()

#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


# Third-party related paths:
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)
set(MYSQL_CONNECTOR_C++_SOURCE_DIR ${THIRD_PARTY_DIR}/mysql-connector-c++-1.1.3)

find_package(Boost REQUIRED)
find_package(Protobuf)

find_library(Tinyxml REQUIRED)
find_library(Iconv REQUIRED)
find_library(Mysqld-dev REQUIRED)
find_library(Mysqlclient-dev REQUIRED)
find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})

include_directories(./src)
include_directories(./src/port)
include_directories(./src/utils)

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

#add_subdirectory(libmodbus)
add_subdirectory(src)
add_subdirectory(third_party)
