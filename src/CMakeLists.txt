#declare source and header files
set( SOURCE_FILES 
         PIAbstractProcessInfoReader.cpp
   )

set( HEADER_FILES 
        PIAbstractProcessInfoReader.h
		PIProcessInfo.h
		PIProcessInfoReader.h
   )

set( HEADER_FILES_PRIVATE

   )

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
   list(APPEND HEADER_FILES PILinuxProcessInfoReader.h)
   list(APPEND SOURCE_FILES PILinuxProcessInfoReader.cpp)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
   list(APPEND HEADER_FILES PIWindowsProcessInfoReader.h)
   list(APPEND HEADER_FILES_PRIVATE WindowsNtApi.h)
   list(APPEND SOURCE_FILES PIWindowsProcessInfoReader.cpp)
endif()

#declare library target
add_library(ProcessInfo STATIC)

#set target sources
target_sources(ProcessInfo
                   PRIVATE ${SOURCE_FILES} ${HEADER_FILES_PRIVATE}
				   PUBLIC ${HEADER_FILES}
			  )	   
			  
#set target includes
target_include_directories(ProcessInfo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#install target
install(
           TARGETS ProcessInfo
           EXPORT ProcessInfo
           ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )

install(FILES ${HEADER_FILES} DESTINATION ${INCLUDE_INSTALL_DIR})