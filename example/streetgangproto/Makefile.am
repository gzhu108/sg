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
    cpp/ByebyeRequest.pb.cc \
    cpp/CreateWorldRequest.pb.cc \
    cpp/CreateWorldResponse.pb.cc \
    cpp/ErrorResponse.pb.cc \
    cpp/GetSceneRequest.pb.cc \
    cpp/GetSceneResponse.pb.cc \
    cpp/GetVersionRequest.pb.cc \
    cpp/GetVersionResponse.pb.cc \
    cpp/MessageHeader.pb.cc \
    cpp/Point.pb.cc \
    cpp/Rectangle.pb.cc

