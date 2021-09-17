cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
  message(" [INFO] VCPKG VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")
endif()

project(MyServer)

# The version number.
set(MyServer_VERSION_MAJOR 1)
set(MyServer_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

find_package(unofficial-http-parser CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

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

if ((CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
  find_library(LIBUUID_LIBRARY libuuid.a)
else()
  set(LIBUUID_LIBRARY "uuid")
endif()

set(EXTRA_LIBS ${EXTRA_LIBS}
  "${DEPENDENCY_LIBS}"
  OpenSSL::SSL OpenSSL::Crypto
  unofficial::http_parser::http_parser
  "${LIBUUID_LIBRARY}"
  )

target_link_libraries(MyServer ${EXTRA_LIBS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(MyServer Threads::Threads nlohmann_json::nlohmann_json)

# add the install targets
install(TARGETS MyServer DESTINATION bin)
