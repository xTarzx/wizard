cmake_minimum_required(VERSION 3.7...3.27)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

if (UNIX AND NOT APPLE)
 set (LINUX TRUE)
endif()

project(WiZard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "-Wall -Werror -std=c++17")

set(JANSSON_DIR ${CMAKE_SOURCE_DIR}/extern/jansson/build)

add_executable(${PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/udp.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${JANSSON_DIR}/include
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${JANSSON_DIR}/lib
)


target_link_libraries(${PROJECT_NAME} jansson)

if(WIN32)
target_link_libraries(${PROJECT_NAME} ws2_32)

endif()