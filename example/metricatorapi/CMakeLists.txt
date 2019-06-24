cmake_minimum_required(VERSION 2.6)
project(MetricatorApi)

# The version number.
set(MetricatorApi_VERSION_MAJOR 1)
set(MetricatorApi_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/MetricatorApiConfig.h.in"
  "${PROJECT_BINARY_DIR}/MetricatorApiConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

add_library(metricatorapi STATIC ${BUILD_INC} ${BUILD_SRC})

set(DEPENDENCY_LIBS
  "microreactor"
  )
  
# add this command to scan dependencies of target
add_dependencies(metricatorapi ${DEPENDENCY_LIBS})

# add the install targets
install(TARGETS metricatorapi DESTINATION bin)
install(FILES ${BUILD_INC} DESTINATION include)
