cmake_minimum_required(VERSION 2.6)
project(StreetGangProto)

set(CMAKE_CXX_STANDARD 17)
 
file(GLOB PB_SRC
  "${PROJECT_SOURCE_DIR}/../*.proto"
  )

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PB_SRC})

add_library(streetgangproto STATIC ${PROTO_SRCS} ${PROTO_HDRS})

# add the install targets
install(TARGETS streetgangproto DESTINATION bin)
install(FILES ${PROTO_HDRS} DESTINATION include)
