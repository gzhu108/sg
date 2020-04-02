cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project(MyServer)

# The version number.
set(MyServer_VERSION_MAJOR 1)
set(MyServer_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/MyServerConfig.h.in"
  "${PROJECT_BINARY_DIR}/MyServerConfig.h"
  )
 
# To find and use dependency header files
find_path(DEPENDENCY_INCLUDE_DIR PATH_SUFFIXES rapidjson)
include_directories(${DEPENDENCY_INCLUDE_DIR})

find_package(unofficial-http-parser CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/generated/*.h"
  "${PROJECT_SOURCE_DIR}/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/generated/*.cpp"
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

link_directories(
  "${PROJECT_BINARY_DIR}/../../microreactor"
  )

add_executable(MyServer ${BUILD_INC} ${BUILD_SRC})

set(DEPENDENCY_LIBS
  "microreactor"
  )

# add this command to scan dependencies of target
add_dependencies(MyServer ${DEPENDENCY_LIBS})

set(EXTRA_LIBS ${EXTRA_LIBS}
  "${DEPENDENCY_LIBS}"
  OpenSSL::SSL OpenSSL::Crypto
  unofficial::http_parser::http_parser
  "uuid"
  )

target_link_libraries(MyServer ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(MyServer Threads::Threads)

# add the install targets
install(TARGETS MyServer DESTINATION bin)
