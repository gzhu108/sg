cmake_minimum_required(VERSION 2.6)
project(StreetGangClient)

# The version number.
set(StreetGangClient_VERSION_MAJOR 1)
set(StreetGangClient_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/StreetGangClientConfig.h.in"
  "${PROJECT_BINARY_DIR}/StreetGangClientConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  "${PROJECT_SOURCE_DIR}/../streetgangproto/cpp"
  "${PROJECT_SOURCE_DIR}/../streetgangapi"
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
  "${PROJECT_BINARY_DIR}/../../microreactor"
  )

add_executable(streetgangclient ${BUILD_INC} ${BUILD_SRC})

set(DEPENDENCY_LIBS
  "streetgangapi"
  "streetgangproto"
  "microreactor"
  )

# add this command to scan dependencies of target
add_dependencies(streetgangclient ${DEPENDENCY_LIBS})

set(EXTRA_LIBS ${EXTRA_LIBS}
  "${DEPENDENCY_LIBS}"
  "${Protobuf_LIBRARIES}"
  "${OPENSSL_LIBRARIES}"
  "uuid"
  "dl"
  )

target_link_libraries(streetgangclient ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(streetgangclient Threads::Threads)

# add the install targets
install(TARGETS streetgangclient DESTINATION bin)
