cmake_minimum_required (VERSION 3.24)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:\\Qt\\6.9.1\\mingw_64")
#give Project a better name
project(ChaosCampProject VERSION 1.0)

find_package(Qt6 COMPONENTS Core  Widgets Designer Gui REQUIRED)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
if (Qt6_FOUND)
    message("found")
endif()


file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/headers/*.hpp")

#qt_wrap_cpp(moc_sources src/mainwindow.cpp)
#add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/src/main.cpp" ${SOURCES})
qt_add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Widgets Qt::Designer Qt::Gui)

#qt_finalize_executable(${PROJECT_NAME})