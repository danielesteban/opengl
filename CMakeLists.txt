cmake_minimum_required(VERSION 3.0)
project(Engine)

add_definitions("-std=c++11")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(
  core
  geometries
  scenes
  shaders
  lib/color/src/color
  lib/FastNoise
  ${CONAN_SRC_DIRS_IMGUI}/bindings
)

file(GLOB Engine_SOURCES
  main.cpp
  "core/*.cpp"
  "geometries/*.cpp"
  "scenes/*.cpp"
  "shaders/*.cpp"
  lib/FastNoise/FastNoise.cpp
  ${CONAN_SRC_DIRS_IMGUI}/bindings/imgui_impl_glfw.cpp
  ${CONAN_SRC_DIRS_IMGUI}/bindings/imgui_impl_opengl3.cpp
)

IF(WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") 
ENDIF()

add_executable(engine ${Engine_SOURCES})
target_link_libraries(engine ${CONAN_LIBS})
