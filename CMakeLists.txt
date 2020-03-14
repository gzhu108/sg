cmake_minimum_required(VERSION 3.9)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
  message(" [INFO] VCPKG CMAKE_TOOLCHAIN_FILE = ${CMAKE_TOOLCHAIN_FILE}")
endif()

if(DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
  set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}" CACHE STRING "")
endif()

project (sg)

add_subdirectory(microreactor)
add_subdirectory(example/simplewebserver)
add_subdirectory(example/discoveryservice)
add_subdirectory(example/metricatorapi)
add_subdirectory(example/metricator)
add_subdirectory(example/bouncer)
add_subdirectory(example/worldapi)
add_subdirectory(example/worldserver)
add_subdirectory(example/streetgangproto)
add_subdirectory(example/streetgangapi)
add_subdirectory(example/streetgangclient)
add_subdirectory(example/streetgangserver)

