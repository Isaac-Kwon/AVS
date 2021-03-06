
#-------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(ALPIDEVerticalSimulation)

# User Options

option(DISABLE_ROOT "Disable ROOT")
option(DISABLE_ROOT_CONFIG "Disable usage of ROOTConfig.cmake")
option(DISABLE_QT "Disable QT")

# by default build optimized code with debug symbols
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

#-------------------------------------------------------
# look for Qt, required unless disabled explicitly
if(DISABLE_QT STREQUAL "OFF")
  message(STATUS "Looking for Qt5")
  find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Xml)
  message(STATUS "Using Qt ${Qt5_VERSION}")
elseif()
  message(STATUS "GUI disabled")
endif()

#-------------------------------------------------------
# look for ROOT and warn if unavailable
if(DISABLE_ROOT STREQUAL "OFF")
  message(STATUS "Looking for ROOT")
  # try first with ROOTConfig.cmake,
  # then fallback to FindROOT.cmake (non-cmake installation of ROOT)
  if(DISABLE_ROOT_CONFIG STREQUAL "OFF")
    # find_package(ROOT CONFIG)
    find_package(ROOT REQUIRED COMPONENTS MathCore Hist Tree)
  endif()
  if(NOT ROOT_FOUND)
    list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
    find_package(ROOT MODULE REQUIRED)
  endif()
else()
  message(STATUS "ROOT disabled")
endif()

if(ROOT_FOUND)
  message(STATUS "Using ROOT ${ROOT_VERSION}")
else()
  message(WARNING "Compilation without ROOT results in limited functionality!")
endif()

#-------------------------------------------------------
# set C++ standard ensuring compatibility with ROOT
set(CMAKE_CXX_STANDARD 11)
if(ROOT_cxx17_FOUND)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  message(STATUS "ROOT_cxx17 Used")
elseif(ROOT_cxx14_FOUND)
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  message(STATUS "ROOT_cxx14 Used")
elseif(ROOT_cxx11_FOUND)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  message(STATUS "ROOT_cxx11 Used")
else()
  message(STATUS "ROOT_cxx11 root used")
endif()
message(STATUS "Using C++${CMAKE_CXX_STANDARD}")

#-------------------------------------------------------
# warn about undefined symbols already for libraries
# link_libraries("-Wl,--as-needed")
# link_libraries("-Wl,--no-undefined")

add_compile_options("-Wall")
add_compile_options("-Wpedantic")

#-------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#-------------------------------------------------------
# check for build in the source tree
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(STATUS "Building in source tree")
  set(BUILDINSOURCE true)
endif()

#-------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Setup ROOT include directories and compile definitions




#-------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#-------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(AVS AVSmain.cc ${sources} ${headers})
target_include_directories(AVS PRIVATE ${ROOT_INCLUDE_DIRS})
target_link_libraries(AVS ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

add_executable(AVS_condor AVScondor.cc ${sources} ${headers})
target_include_directories(AVS_condor PRIVATE ${ROOT_INCLUDE_DIRS})
target_link_libraries(AVS_condor ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})


SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-variable -Wunused-parameter")
include(CPack)
#-------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SIMULATION_SCRIPTS
  init_vis.mac
  vis.mac
  )

foreach(_script ${SIMULATION_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

file(COPY macro DESTINATION .)

#-------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(B1 DEPENDS AVS)

#-------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
# install(TARGETS AVSmain DESTINATION bin)