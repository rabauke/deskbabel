cmake_minimum_required(VERSION 3.23)

project(DeskBabel VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 COMPONENTS Core Quick LinguistTools Concurrent REQUIRED)
qt_standard_project_setup()

add_subdirectory(src)
