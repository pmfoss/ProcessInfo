SET(EXAMPLE_SRC_FILES
   SimpleExample.cpp)

SET(LINK_LIBRARIES
       ProcessInfo)
       
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
   list(APPEND LINK_LIBRARIES ntdll)
endif()

foreach(EXAMPLE_SRC_FILE IN LISTS EXAMPLE_SRC_FILES)
    get_filename_component(OUTPUT_FILE ${EXAMPLE_SRC_FILE} NAME_WLE)
    add_executable(${OUTPUT_FILE} ${EXAMPLE_SRC_FILE})
    target_link_libraries(${OUTPUT_FILE} PRIVATE ${LINK_LIBRARIES})
endforeach()