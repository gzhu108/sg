AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++17 \
    -I../../tools/json \
    -I../../microreactor/src \
    -I../streetgangproto/cpp

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
lib_LIBRARIES = \
    $(top_builddir)/lib/libstreetgangapi.a

__top_builddir__lib_libstreetgangapi_a_SOURCES = \
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
    RestRequestByebye.cpp \
    RestRequestCreateWorld.cpp \
    RestRequestGetScene.cpp \
    RestRequestGetVersion.cpp \
    RestResponseCreateWorld.cpp \
    RestResponseError.cpp \
    RestResponseGetScene.cpp \
    RestResponseGetVersion.cpp \
    RestStreetGangResponder.cpp
