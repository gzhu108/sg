cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
  message(" [INFO] VCPKG VCPKG_TARGET_TRIPLET = ${VCPKG_TARGET_TRIPLET}")
endif()

project(StreetGangProto)

set(CMAKE_CXX_STANDARD 17)
 
# To find and use dependency header files
find_path(DEPENDENCY_INCLUDE_DIR NAMES http_parser.h)
include_directories(${DEPENDENCY_INCLUDE_DIR})

file(GLOB PB_SRC "${PROJECT_SOURCE_DIR}/*.proto")

#Code Generation
if(protobuf_MODULE_COMPATIBLE) #Legacy Suppport
  protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PB_SRC})
endif()

add_library(streetgangproto STATIC ${PB_SRC})

find_package(Protobuf CONFIG REQUIRED)
protobuf_generate(LANGUAGE cpp TARGET streetgangproto)

# add the install targets
install(TARGETS streetgangproto DESTINATION bin)
install(FILES ${PROTO_HDRS} DESTINATION include)
