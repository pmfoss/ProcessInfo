SET(LIBRARY_SRC_FILES
   AbstractProcessDataReader.cpp)

message(STATUS "CMake: OS " ${CMAKE_SYSTEM_NAME})
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   list(APPEND LIBRARY_SRC_FILES LinuxProcessDataReader.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
   list(APPEND LIBRARY_SRC_FILES WindowsProcessDataReader.cpp)
endif()

add_library(ProcessData STATIC ${LIBRARY_SRC_FILES})