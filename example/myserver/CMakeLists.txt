cmake_minimum_required (VERSION 2.6)
project (myserver)

# add the binary tree to the search path for include files
include_directories ("${PROJECT_BINARY_DIR}/generated")
	add_subdirectory (generated)
	set (EXTRA_LIBS ${EXTRA_LIBS} generated)

add_executable(myserver main.cpp)
target_link_libraries (myserver microreactor)
