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
    protobuf/cpp/Rectangle.pb.cc \
    BinaryRequestByebye.cpp \
    BinaryRequestCreateWorld.cpp \
    BinaryRequestGetScene.cpp \
    BinaryRequestGetVersion.cpp \
    BinaryResponseCreateWorld.cpp \
    BinaryResponseError.cpp \
    BinaryResponseGetScene.cpp \
    BinaryResponseGetVersion.cpp \
    BinaryStreetGangRequester.cpp \
    BinaryStreetGangResponder.cpp \
    MessageBase.cpp \
    PBRequestByebye.cpp \
    PBRequestCreateWorld.cpp \
    PBRequestGetScene.cpp \
    PBRequestGetVersion.cpp \
    PBResponseCreateWorld.cpp \
    PBResponseError.cpp \
    PBResponseGetScene.cpp \
    PBResponseGetVersion.cpp \
    PBStreetGangRequester.cpp \
    PBStreetGangResponder.cpp \
    RequestByebye.cpp \
    RequestCreateWorld.cpp \
    RequestGetScene.cpp \
    RequestGetVersion.cpp \
    ResponseCreateWorld.cpp \
    ResponseError.cpp \
    ResponseGetScene.cpp \
    ResponseGetVersion.cpp \
    StreetGangRestResponseJsonEncoder.cpp \
    StreetGangRestResponseTextEncoder.cpp