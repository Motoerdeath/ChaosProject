cmake_minimum_required (VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#give Project a better name
project(ChaosCampProject VERSION 1.0)



set(CCP_SOURCE_FILES 
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  ${CMAKE_SOURCE_DIR}/src/crtImage.cpp
)


set(Project_SOURCE_DIR ${CMAKE_SOURCE_DIR})



add_executable(${PROJECT_NAME} ${CCP_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
