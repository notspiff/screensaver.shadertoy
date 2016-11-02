project(screensaver.shadertoy)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})

enable_language(CXX)

find_package(Kodi REQUIRED)
find_package(p8-platform REQUIRED)

set(INCLUDES ${KODI_INCLUDE_DIR} ${p8-platform_INCLUDE_DIRS})

if(WIN32)
  set(SHADERTOY_SOURCES src/toys.cpp)
else()
  set(SHADERTOY_SOURCES src/lodepng.cpp
                         src/toysgl.cpp)
  find_package(OpenGL)
  if(OPENGL_FOUND)
   list(APPEND INCLUDES ${OpenGL_INCLUDE_DIR})
   add_definitions(-DHAS_OPENGL)
   set(DEPLIBS ${OPENGL_LIBRARIES})
  else()
   find_package(OpenGLES2)
   if(OPENGLES2_FOUND)
     list(APPEND INCLUDES ${OPENGLES2_INCLUDE_DIR})
     add_definitions(-DHAS_GLES2 -DHAS_GLES ${OpenGLES2_CFLAGS_OTHER})
     set(DEPLIBS ${OPENGLES2_LIBRARIES})
   else()
     message(FATAL_ERROR "No rendering API found. Bailing")
   endif()
  endif()
endif()

include_directories(${INCLUDES})

build_addon(screensaver.shadertoy SHADERTOY DEPLIBS)

include(CPack)
