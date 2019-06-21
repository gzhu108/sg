cmake_minimum_required(VERSION 2.6)
project(StreetGangApi)

# The version number.
set(StreetGangApi_VERSION_MAJOR 1)
set(StreetGangApi_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/StreetGangApiConfig.h.in"
  "${PROJECT_BINARY_DIR}/StreetGangApiConfig.h"
  )

# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  "${PROJECT_SOURCE_DIR}/../streetgangproto/cpp"
  "${PROJECT_SOURCE_DIR}/../../tools/json"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

add_library(streetgangapi STATIC ${BUILD_INC} ${BUILD_SRC})

# add the install targets
install(TARGETS streetgangapi DESTINATION bin)
install(FILES ${BUILD_INC} DESTINATION include)

