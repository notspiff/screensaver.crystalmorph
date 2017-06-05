project(screensaver.crystalmorph)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${KODI_INCLUDE_DIR})

set(CRYSTALMORPH_SOURCES src/fractalcontroller.cpp
                         src/Fractal.cpp
                         src/fractalobject.cpp
                         src/sphere.cpp
                         src/Util.cpp)

set(CRYSTALMORPH_HEADERS src/Fractal.h
                         src/types.h
                         src/fractalcontroller.h
                         src/Util.h
                         src/sphere.h
                         src/fractalobject.h)

set(DEPLIBS ${OPENGL_LIBRARIES})

build_addon(screensaver.crystalmorph CRYSTALMORPH DEPLIBS)

include(CPack)
