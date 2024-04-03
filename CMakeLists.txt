# ubuntu 18.04 cmake version is 3.10.2

cmake_minimum_required(VERSION 3.10)

project(unit_test)

set(CMAKE_CXX_STANDARD 11)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

message(${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
add_subdirectory(${PROJECT_DIR}/googletest gtest)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-pmf-conversions -fpermissive -Wno-noexcept-type -Wno-unused-function")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-inline -fno-access-control -fprofile-arcs -ftest-coverage")


include_directories(${PROJECT_DIR}/src)

include_directories(${PROJECT_DIR}/stub)
include_directories(${PROJECT_DIR}/googletest/googletest/include)
include_directories(${PROJECT_DIR}/googletest/googlemock/include)

link_libraries(-lrt -lm gtest gmock gtest_main -lpthread)

# add_subdirectory(src)
file(GLOB UNITEST_SOURCES src/*.cpp)
add_executable(unit_test ${UNITEST_SOURCES})
