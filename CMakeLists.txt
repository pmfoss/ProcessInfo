cmake_minimum_required(VERSION 3.15)

project(
  ProcessInfo
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

option(PROCESSINFO_PKGCONFIG "Build pkg-config .pc file for ProcessInfo library." OFF)
option(PROCESSINFO_EXAMPLES "Build examples for ProcessInfo library." OFF)

set(INCLUDE_INSTALL_DIR
      "${CMAKE_INSTALL_INCLUDEDIR}/ProcessInfo"
	  CACHE PATH "The directory relative to CMAKE_INSTALL_PREFIX where ProcessInfo header files are installed")


set(PKGCONFIG_INSTALL_DIR
    "${CMAKE_INSTALL_DATADIR}/pkgconfig"
    CACHE PATH "The directory relative to CMAKE_INSTALL_PREFIX where processinfo.pc is installed"
   )

foreach(var INCLUDE_INSTALL_DIR CMAKEPACKAGE_INSTALL_DIR PKGCONFIG_INSTALL_DIR)
  # If an absolute path is specified, make it relative to "{CMAKE_INSTALL_PREFIX}".
  if(IS_ABSOLUTE "${${var}}")
    file(RELATIVE_PATH "${var}" "${CMAKE_INSTALL_PREFIX}" "${${var}}")
  endif()
endforeach()

add_subdirectory(src)

if(PROCESSINFO_EXAMPLES)
    include_directories(src)
    link_directories(${CMAKE_BINARY_DIR}/src)
    add_subdirectory(examples)
endif()

if(COROUTINECLASSES_PKGCONFIG)
    configure_file(processinfo.pc.in processinfo.pc @ONLY)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/processinfo.pc
        DESTINATION ${PKGCONFIG_INSTALL_DIR})
endif()