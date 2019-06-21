cmake_minimum_required(VERSION 2.6)
project(SimpleWebServer)

# The version number.
set(SimpleWebServer_VERSION_MAJOR 1)
set(SimpleWebServer_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/SimpleWebServerConfig.h.in"
  "${PROJECT_BINARY_DIR}/SimpleWebServerConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../tools/json "
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

link_directories(
  "${PROJECT_BINARY_DIR}/../../microreactor"
  )

add_executable(simplewebserver ${BUILD_INC} ${BUILD_SRC})

set(DEPENDENCY_LIBS
  "microreactor"
  )

# add this command to scan dependencies of target
add_dependencies(simplewebserver ${DEPENDENCY_LIBS})

set(EXTRA_LIBS ${EXTRA_LIBS}
  "${DEPENDENCY_LIBS}"
  "ssl"
  "crypto"
  "uuid"
  "dl"
  )

target_link_libraries(simplewebserver ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(simplewebserver Threads::Threads)

# add the install targets
install(TARGETS simplewebserver DESTINATION bin)
