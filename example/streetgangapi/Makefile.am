AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -O2 -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++11 \
    -I../../tools/protobuf/linux/include \
    -I../../tools/json \
    -I../../microreactor/src

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
lib_LIBRARIES = \
    $(top_builddir)/lib/libstreetgangapi.a

__top_builddir__lib_libstreetgangapi_a_SOURCES = \
    protobuf/cpp/GetVersionResponse.pb.cc \
    protobuf/cpp/CreateWorldRequest.pb.cc \
    protobuf/cpp/CreateWorldResponse.pb.cc \
    protobuf/cpp/GetSceneRequest.pb.cc \
    protobuf/cpp/GetSceneResponse.pb.cc \
    protobuf/cpp/ErrorResponse.pb.cc \
    protobuf/cpp/MessageHeader.pb.cc \
    protobuf/cpp/Rectangle.pb.cc \
    protobuf/cpp/Point.pb.cc \
    protobuf/cpp/StreetGangPBService.pb.cc \
    MessageBase.cpp \
    RequestByebye.cpp \
    RequestCreateWorld.cpp \
    RequestGetScene.cpp \
    RequestGetVersion.cpp \
    ResponseError.cpp \
    ResponseCreateWorld.cpp \
    ResponseGetScene.cpp \
    ResponseGetVersion.cpp \
    StreetGangApi.cpp \
    StreetGangMessageRequestEncoder.cpp \
    StreetGangMessageResponseEncoder.cpp \
    StreetGangPBRequestEncoder.cpp \
    StreetGangPBResponseEncoder.cpp \
    StreetGangRestResponseTextEncoder.cpp \
    StreetGangRestResponseJsonEncoder.cpp \
    BinaryRequestCreateWorld.cpp
