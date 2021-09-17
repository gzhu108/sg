cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
  message(" [INFO] VCPKG VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")
endif()

project(StreetGangClient)

# The version number.
set(StreetGangClient_VERSION_MAJOR 1)
set(StreetGangClient_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 11)
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  "${CMAKE_CURRENT_BINARY_DIR}/../streetgangproto"
  "${PROJECT_SOURCE_DIR}/../streetgangapi"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

find_package(Protobuf CONFIG REQUIRED)

# To find and use dependency header files
find_path(DEPENDENCY_INCLUDE_DIR PATH_SUFFIXES openssl)
include_directories(${DEPENDENCY_INCLUDE_DIR})

find_package(unofficial-http-parser CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

link_directories(
  "${PROJECT_BINARY_DIR}/../streetgangproto"
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

if ((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
  find_library(LIBUUID_LIBRARY libuuid.a)
else()
  set(LIBUUID_LIBRARY "uuid")
endif()

set(EXTRA_LIBS ${EXTRA_LIBS}
  "${DEPENDENCY_LIBS}"
  protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
  OpenSSL::SSL OpenSSL::Crypto
  unofficial::http_parser::http_parser
  "${LIBUUID_LIBRARY}"
  )

target_link_libraries(streetgangclient ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(streetgangclient Threads::Threads)

# add the install targets
install(TARGETS streetgangclient DESTINATION bin)
