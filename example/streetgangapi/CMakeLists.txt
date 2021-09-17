cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
  message(" [INFO] VCPKG VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")
endif()

project(StreetGangApi)

# The version number.
set(StreetGangApi_VERSION_MAJOR 1)
set(StreetGangApi_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 14)

# To find and use dependency header files
find_path(DEPENDENCY_INCLUDE_DIR PATH_SUFFIXES openssl)
include_directories(${DEPENDENCY_INCLUDE_DIR})

# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/../../microreactor/src"
  "${CMAKE_CURRENT_BINARY_DIR}/../streetgangproto"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/*.cpp"
  )

add_library(streetgangapi STATIC ${BUILD_INC} ${BUILD_SRC})

set(DEPENDENCY_LIBS
  "streetgangproto"
  "microreactor"
  )

# add this command to scan dependencies of target
add_dependencies(streetgangapi ${DEPENDENCY_LIBS})

# add the install targets
install(TARGETS streetgangapi DESTINATION bin)
install(FILES ${BUILD_INC} DESTINATION include)

