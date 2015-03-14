project(dbpsk)

cmake_minimum_required(VERSION 2.8)

include(GNUInstallDirs)
mark_as_advanced(CLEAR
    CMAKE_INSTALL_BINDIR
    CMAKE_INSTALL_INCLUDEDIR
    CMAKE_INSTALL_LIBDIR
)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds in ${CMAKE_BINARY_DIR} are not "
   "allowed, please remove ./CMakeCache.txt and ./CMakeFiles/, create a "
   "separate build directory and run cmake from there.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")


set(DBPSK_VERSION_MAJOR 0)
set(DBPSK_VERSION_MINOR 1)

set(DBPSK_VERSION "${DBPSK_VERSION_MAJOR}.${DBPSK_VERSION_MINOR}")
set(SOVERSION "${DBPSK_VERSION_MAJOR}.${DBPSK_VERSION_MINOR}")


message(STATUS "dbpsk version: ${DBPSK_VERSION}")


# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set default C++ flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")


message(STATUS "Build type is: " ${CMAKE_BUILD_TYPE})
message(STATUS "Compiler Flags: " ${CMAKE_CXX_FLAGS})


include(CheckIncludeFiles)
check_include_files("stdlib.h" HAVE_STDLIB_H)
check_include_files("string.h" HAVE_STRING_H)
check_include_files("stdio.h" HAVE_STDIO_H)
check_include_files("stdint.h" HAVE_STDINT_H)
check_include_files("math.h" HAVE_STDMATH_H)



include(CheckFunctionExists)
check_function_exists(memset  HAVE_MEMSET)


#configure_file ("${PROJECT_SOURCE_DIR}/cmake/config.h.in"
 #               "${PROJECT_BINARY_DIR}/config.h" )
include_directories(${PROJECT_BINARY_DIR})


add_subdirectory(src)







