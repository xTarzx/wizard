cmake_minimum_required(VERSION 3.7...3.27)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

if (UNIX AND NOT APPLE)
 set (LINUX TRUE)
endif()

project(WiZard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++17")

add_executable(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)