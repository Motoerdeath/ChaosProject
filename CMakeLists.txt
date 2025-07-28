cmake_minimum_required (VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#give Project a better name
project(ChaosCampProject VERSION 1.0)


file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")


add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/src/main.cpp" ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

