cmake_minimum_required (VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#give Project a better name
project(ChaosCampProject VERSION 1.0)



set(CCP_SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/main.cpp 
  ${CMAKE_SOURCE_DIR}/src/jsonUtilities.cpp
  ${CMAKE_SOURCE_DIR}/src/scene.cpp
  ${CMAKE_SOURCE_DIR}/src/crtImage.cpp
  ${CMAKE_SOURCE_DIR}/src/camera.cpp
  ${CMAKE_SOURCE_DIR}/src/ppmImage.cpp
  ${CMAKE_SOURCE_DIR}/src/matrix.cpp
  ${CMAKE_SOURCE_DIR}/src/mesh.cpp
  ${CMAKE_SOURCE_DIR}/src/triangle.cpp
  ${CMAKE_SOURCE_DIR}/src/ray.cpp
  ${CMAKE_SOURCE_DIR}/src/renderer.cpp
  ${CMAKE_SOURCE_DIR}/src/texture.cpp
  ${CMAKE_SOURCE_DIR}/src/bucketqueue.cpp
  ${CMAKE_SOURCE_DIR}/src/AABB.cpp
  ${CMAKE_SOURCE_DIR}/src/accelerationStructure.cpp
  ${CMAKE_SOURCE_DIR}/src/environmentMap.cpp

)


set(Project_SOURCE_DIR ${CMAKE_SOURCE_DIR})
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")



#add_executable(${PROJECT_NAME} ${CCP_SOURCE_FILES})
add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/src/main.cpp" ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)

