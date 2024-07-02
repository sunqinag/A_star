cmake_minimum_required(VERSION 3.0.0)
project(A_star VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 ")
SET(CMAKE_BUILD_TYPE "Debug")

find_package(OpenCV)

include_directories(
    "/usr/include/eigen3"
    ${OpenCV_INCLUDE_DIRS}
)

add_executable(A_star main.cpp a_star.cpp)
target_link_libraries(A_star ${OpenCV_LIBS})

add_executable(test_algorithm test_algorithm.cpp a_star.cpp)
target_link_libraries(test_algorithm ${OpenCV_LIBS})
