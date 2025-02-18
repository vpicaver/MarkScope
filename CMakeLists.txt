cmake_minimum_required(VERSION 3.16)

project(MarkScope LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)

add_library(MarkScope SHARED
  MarkScope/MarkScope_global.h
  MarkScope/Profiler.h
  MarkScope/FrameProfiler.h
  FrameProfiler.cpp
)

target_include_directories(MarkScope PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MarkScope
    PUBLIC
    Qt::Core
    Qt::Quick
)

target_compile_definitions(MarkScope PRIVATE MARKSCOPE_LIBRARY)
