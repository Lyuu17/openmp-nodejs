
add_subdirectory(json)
add_subdirectory(open.mp-sdk)
add_subdirectory(node)

if(WIN32)
add_subdirectory(minhook EXCLUDE_FROM_ALL)
endif()
