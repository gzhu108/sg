cmake_minimum_required(VERSION 2.6)
project(Microreactor)

# The version number.
set(Microreactor_VERSION_MAJOR 1)
set(Microreactor_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 17)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file(
  "${PROJECT_SOURCE_DIR}/MicroreactorConfig.h.in"
  "${PROJECT_BINARY_DIR}/MicroreactorConfig.h"
  )
 
# add the binary tree to the search path for include files
include_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_SOURCE_DIR}/test"
  "${PROJECT_SOURCE_DIR}/../tools/rapidxml"
  "${PROJECT_SOURCE_DIR}/../tools/http-parser"
  )

file(GLOB BUILD_INC
  "${PROJECT_SOURCE_DIR}/src/*.h"
  )

file(GLOB BUILD_SRC
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
  )

set(ADDITIONAL_SRC
  "${PROJECT_SOURCE_DIR}/../tools/http-parser/http_parser.c"
  )

add_library(microreactor STATIC ${BUILD_INC} ${BUILD_SRC} ${ADDITIONAL_SRC})

# add the install targets
install(TARGETS microreactor DESTINATION bin)
install(FILES ${BUILD_INC} DESTINATION include)

# Enable testing
enable_testing()
find_package(GTest REQUIRED)

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
target_link_libraries(microreactor_test Threads::Threads)

target_link_libraries(microreactor_test
  "microreactor"  
  "uuid"
  "${GTEST_LIBRARIES}"
  ) 

gtest_discover_tests(microreactor_test)

add_custom_command(TARGET microreactor_test
  POST_BUILD
  COMMAND microreactor_test
  )

