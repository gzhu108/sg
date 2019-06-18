cmake_minimum_required(VERSION 2.6)
project(Bouncer)

# The version number.
set(Bouncer_VERSION_MAJOR 1)
set(Bouncer_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/BouncerConfig.h.in"
  "${PROJECT_BINARY_DIR}/BouncerConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  "${PROJECT_SOURCE_DIR}/../../tools/json"
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

set(EXTRA_LIBS ${EXTRA_LIBS}
  "microreactor"
)

add_executable(bouncer ${BUILD_INC} ${BUILD_SRC})
target_link_libraries(bouncer ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(bouncer Threads::Threads)

# add the install targets
install(TARGETS bouncer DESTINATION bin)
