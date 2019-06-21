AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++17

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
lib_LIBRARIES = \
    $(top_builddir)/lib/libstreetgangproto.a

__top_builddir__lib_libstreetgangproto_a_SOURCES = \
    protobuf/cpp/ByebyeRequest.pb.cc \
    protobuf/cpp/CreateWorldRequest.pb.cc \
    protobuf/cpp/CreateWorldResponse.pb.cc \
    protobuf/cpp/ErrorResponse.pb.cc \
    protobuf/cpp/GetSceneRequest.pb.cc \
    protobuf/cpp/GetSceneResponse.pb.cc \
    protobuf/cpp/GetVersionRequest.pb.cc \
    protobuf/cpp/GetVersionResponse.pb.cc \
    protobuf/cpp/MessageHeader.pb.cc \
    protobuf/cpp/Point.pb.cc \
    protobuf/cpp/Rectangle.pb.cc

