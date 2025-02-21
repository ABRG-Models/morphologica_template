#
# This is an example CMakeLists.txt file to compile a program against
# a morphologica that is 'in-tree'
#

cmake_minimum_required(VERSION 3.10)

# Give your project a name
project(morphologica_template)

# From CMAKE_SYSTEM work out which of __OSX__, __GLN__, __NIX__ are required
if(CMAKE_SYSTEM MATCHES Linux.*)
  set(OS_FLAG "-D__GLN__")
elseif(CMAKE_SYSTEM MATCHES BSD.*)
  set(OS_FLAG "-D__NIX__")
elseif(APPLE)
  set(OS_FLAG "-D__OSX__")
else()
  message(ERROR "Operating system not supported: " ${CMAKE_SYSTEM})
endif()

# morphologica uses c++-20 language features
set(CMAKE_CXX_STANDARD 20)

# Add the host definition to CXXFLAGS along with other switches, depending on OS/Compiler
if (APPLE)
  set(COMPREHENSIVE_WARNING_FLAGS "-Wall -Wfatal-errors")
else()
  # This assumes a gcc compiler (or a gcc mimic like Clang)
  set(COMPREHENSIVE_WARNING_FLAGS "-Wall -Wextra -Wpedantic -pedantic-errors -Werror -Wfatal-errors -Wno-psabi -Wno-unknown-pragmas")
endif()
set(CMAKE_CXX_FLAGS "${OS_FLAG} -g -O3 ${COMPREHENSIVE_WARNING_FLAGS}")

# Additional GL compiler flags.
#
set(OpenGL_GL_PREFERENCE "GLVND") # Following `cmake --help-policy CMP0072`
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL3_PROTOTYPES -DGL_GLEXT_PROTOTYPES")
if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGL_SILENCE_DEPRECATION")
endif()

# Tell the program where the morph fonts are, to compile them into the binary
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMORPH_FONTS_DIR=\"\\\"${PROJECT_SOURCE_DIR}/morphologica/fonts\\\"\"")

# Find the libraries which will be needed by morphologica
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(Armadillo) # Optional
find_package(HDF5) # Optional

# include files for graphics
set(MORPH_INC_EXTRA ${ARMADILLO_INCLUDE_DIR} ${ARMADILLO_INCLUDE_DIRS} ${HDF5_INCLUDE_DIR})
include_directories(${MORPH_INC_EXTRA} ${OPENGL_INCLUDE_DIR} ${GLFW3_INCLUDE_DIR} ${FREETYPE_INCLUDE_DIRS})
# Assuming that you installed morphologica in-tree (i.e. 'next to' yourprog.cpp).
set(MORPH_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/morphologica" CACHE PATH "The path to morphologica")
include_directories(BEFORE ${MORPH_INCLUDE_PATH}/include) # Allows GL3/gl3.h to be found
include_directories(BEFORE ${MORPH_INCLUDE_PATH})         # Allows morph/Header.h to be found

# If you want to add another directory to your include path, you can add an extra include_directories call:
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/my_includes)

#
# All the library finding is now done. It's time to compile the program.
#

# The sample is compiled from a single c++ file
add_executable(prog1 prog1.cpp)

# Morphologica code requires a number of libraries, these are the ones required for graphics
target_link_libraries(prog1 OpenGL::GL Freetype::Freetype glfw)
# If you change prog1 to use morph::HdfData or morph::BezCurve, you may need to link these too:
# set(MORPH_LIBS_EXTRA ${ARMADILLO_LIBRARY} ${ARMADILLO_LIBRARIES} ${HDF5_C_LIBRARIES})

#
# All done!
#

# This is just a useful stanza for debugging of cmake variables:
option(DEBUG_VARIABLES OFF)
if(DEBUG_VARIABLES)
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif(DEBUG_VARIABLES)
