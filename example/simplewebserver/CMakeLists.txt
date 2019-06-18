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
# so that we will find TutorialConfig.h
include_directories(
  "${PROJECT_BINARY_DIR}"
  "../../tools/json "
  "../../microreactor/src"
  )

file(GLOB BUILD_INC
  "*.h"
  )

file(GLOB BUILD_SRC
  "*.cpp"
  )

set(EXTRA_LIBS ${EXTRA_LIBS}
  "${PROJECT_SOURCE_DIR}/../../microreactor/libmicroreactor.a"
  ssl
  crypto
  uuid
  dl
)

add_executable(simplewebserver ${BUILD_INC} ${BUILD_SRC})
target_link_libraries(simplewebserver ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(simplewebserver Threads::Threads)

# add the install targets
install(TARGETS simplewebserver DESTINATION bin)
