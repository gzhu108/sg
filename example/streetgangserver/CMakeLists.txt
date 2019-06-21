cmake_minimum_required(VERSION 2.6)
project(StreetGangServer)

# The version number.
set(StreetGangServer_VERSION_MAJOR 1)
set(StreetGangServer_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/StreetGangServerConfig.h.in"
  "${PROJECT_BINARY_DIR}/StreetGangServerConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  "${PROJECT_SOURCE_DIR}/.."
  "${PROJECT_SOURCE_DIR}/../streetgangproto/cpp"
  "${PROJECT_SOURCE_DIR}/../streetgangapi"
  "${PROJECT_SOURCE_DIR}/../discoveryservice"
  "${PROJECT_SOURCE_DIR}/../worldapi"
  "${PROJECT_SOURCE_DIR}/../metricatorapi"
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

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

link_directories(
  "${PROJECT_BINARY_DIR}/../streetgangproto/cpp"
  "${PROJECT_BINARY_DIR}/../streetgangapi"
  "${PROJECT_BINARY_DIR}/../metricatorapi"
  "${PROJECT_BINARY_DIR}/../worldapi"
  "${PROJECT_BINARY_DIR}/../discoveryservice"
  "${PROJECT_BINARY_DIR}/../../microreactor"
  )

set(EXTRA_LIBS ${EXTRA_LIBS}
  "streetgangapi"
  "streetgangproto"
  "metricatorapi"
  "worldapi"
  "discoveryservice"
  "microreactor"
  "${Protobuf_LIBRARIES}"
  "${OPENSSL_LIBRARIES}"
  "uuid"
  "dl"
)

add_executable(streetgangserver ${BUILD_INC} ${BUILD_SRC})
target_link_libraries(streetgangserver ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(streetgangserver Threads::Threads)

# add the install targets
install(TARGETS streetgangserver DESTINATION bin)
