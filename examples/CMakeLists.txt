SET(EXAMPLE_SRC_FILES
   SimpleExample.cpp)

foreach(EXAMPLE_SRC_FILE IN LISTS EXAMPLE_SRC_FILES)
    get_filename_component(OUTPUT_FILE ${EXAMPLE_SRC_FILE} NAME_WLE)
    add_executable(${OUTPUT_FILE} ${EXAMPLE_SRC_FILE})
    target_link_libraries(${OUTPUT_FILE} PRIVATE ProcessData)
endforeach()