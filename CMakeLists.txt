cmake_minimum_required(VERSION 3.23)

project(DeskBabel VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set strict compiler settings for standard conformance
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  # turn on (almost) all compiler warnings and treat them as errors
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
  # enable STL bound checks in debug builds
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_ASSERTIONS")
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
  # turn on (almost) all compiler warnings and treat them as errors
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /W4 /WX")
endif()

find_package(Qt6 COMPONENTS Core Quick LinguistTools Concurrent REQUIRED)
qt_standard_project_setup()

add_subdirectory(src)
