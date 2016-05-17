project(screensaver.crystalmorph)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)

include_directories(${XBMC_INCLUDE_DIR})

set(CRYSTALMORPH_SOURCES src/fractalcontroller.cpp
                         src/Fractal.cpp
                         src/fractalobject.cpp
                         src/sphere.cpp
                         src/Util.cpp)

set(DEPLIBS)

build_addon(screensaver.crystalmorph CRYSTALMORPH DEPLIBS)

include(CPack)
