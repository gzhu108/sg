cmake_minimum_required(VERSION 2.6)
project(Metricator)

# The version number.
set(Metricator_VERSION_MAJOR 1)
set(Metricator_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/MetricatorConfig.h.in"
  "${PROJECT_BINARY_DIR}/MetricatorConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  "${PROJECT_SOURCE_DIR}/../metricatorapi"
  "${PROJECT_SOURCE_DIR}/../discoveryservice"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

link_directories(
  "${PROJECT_BINARY_DIR}/../metricatorapi"
  "${PROJECT_BINARY_DIR}/../discoveryservice"
  "${PROJECT_BINARY_DIR}/../../microreactor"
  )

add_executable(metricator ${BUILD_INC} ${BUILD_SRC})

set(DEPENDENCY_LIBS
  "metricatorapi"
  "discoveryservice"
  "microreactor"
  )

# add this command to scan dependencies of target
add_dependencies(metricator ${DEPENDENCY_LIBS})

set(EXTRA_LIBS ${EXTRA_LIBS}
  "${DEPENDENCY_LIBS}"
  "uuid"
  )

target_link_libraries(metricator ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(metricator Threads::Threads)

# add the install targets
install(TARGETS metricator DESTINATION bin)
