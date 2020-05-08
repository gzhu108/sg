cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
  message(" [INFO] VCPKG VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")
endif()

project(Microreactor)

# The version number.
set(Microreactor_VERSION_MAJOR 1)
set(Microreactor_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)

# To find and use dependency header files
find_path(DEPENDENCY_INCLUDE_DIR NAMES http_parser.h PATH_SUFFIXES rapidxml openssl)
include_directories(${DEPENDENCY_INCLUDE_DIR})
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_SOURCE_DIR}/test"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/src/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
  )

add_library(microreactor STATIC ${BUILD_INC} ${BUILD_SRC})

# add the install targets
install(TARGETS microreactor DESTINATION bin)
install(FILES ${BUILD_INC} DESTINATION include)

# Enable testing
enable_testing()
find_package(GTest CONFIG REQUIRED)

file(GLOB TEST_SRC
  "${PROJECT_SOURCE_DIR}/test/*.cpp"
  )

link_directories(
  "${PROJECT_BINARY_DIR}"
  )

include(GoogleTest)
add_executable(microreactor_test ${TEST_SRC})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(unofficial-http-parser CONFIG REQUIRED)

set(DEPENDENCY_LIBS
  "microreactor"
  )
  
# add this command to scan dependencies of target
add_dependencies(microreactor_test ${DEPENDENCY_LIBS})

set(GTEST_LIBRARIES GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

if ((VCPKG_CMAKE_SYSTEM_NAME STREQUAL "Linux") OR (VCPKG_CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
  find_library(LIBUUID_LIBRARY libuuid.a)
else()
  set(LIBUUID_LIBRARY "uuid")
endif()

set(EXTRA_LIBS ${EXTRA_LIBS}
  "${DEPENDENCY_LIBS}"
  "${GTEST_LIBRARIES}"
  unofficial::http_parser::http_parser
  "Threads::Threads"
  "${LIBUUID_LIBRARY}"
  )
  
target_link_libraries(microreactor_test ${EXTRA_LIBS})

gtest_discover_tests(microreactor_test)

add_custom_command(TARGET microreactor_test
  POST_BUILD
  COMMAND microreactor_test
  )

