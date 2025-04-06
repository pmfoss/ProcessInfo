SET(LIBRARY_SRC_FILES
   AbstractProcessInfoReader.cpp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   list(APPEND LIBRARY_SRC_FILES LinuxProcessInfoReader.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
   list(APPEND LIBRARY_SRC_FILES WindowsProcessInfoReader.cpp)
endif()

add_library(ProcessInfo STATIC ${LIBRARY_SRC_FILES})