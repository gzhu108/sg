cmake_minimum_required(VERSION 2.6)
project(WorldServer)

# The version number.
set(WorldServer_VERSION_MAJOR 1)
set(WorldServer_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/WorldServerConfig.h.in"
  "${PROJECT_BINARY_DIR}/WorldServerConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  "${PROJECT_SOURCE_DIR}/../worldapi"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

link_directories(
  "${PROJECT_BINARY_DIR}/../worldapi"
  "${PROJECT_BINARY_DIR}/../../microreactor"
  )

set(EXTRA_LIBS ${EXTRA_LIBS}
  "worldapi"
  "microreactor"
)

add_executable(worldserver ${BUILD_INC} ${BUILD_SRC})
target_link_libraries(worldserver ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(worldserver Threads::Threads)

# add the install targets
install(TARGETS worldserver DESTINATION bin)
