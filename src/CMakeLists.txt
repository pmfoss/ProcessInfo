SET(LIBRARY_SRC_FILES
   PIAbstractProcessInfoReader.cpp)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   list(APPEND LIBRARY_SRC_FILES PILinuxProcessInfoReader.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
   list(APPEND LIBRARY_SRC_FILES PIWindowsProcessInfoReader.cpp)
endif()

add_library(ProcessInfo STATIC ${LIBRARY_SRC_FILES})