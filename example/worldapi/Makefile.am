AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++17 \
    -I../../microreactor/src

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
lib_LIBRARIES = \
    $(top_builddir)/lib/libworldapi.a

nobase_include_HEADERS = \
    WorldMessage.h \
    Point.h \
    Rectangle.h \
    RequestCreateWorld.h \
    RequestGetWorld.h \
    ResponseCreateWorld.h \
    ResponseError.h \
    ResponseGetWorld.h \
    World.h \
    WorldCache.h \
    WorldId.h \
    WorldRequester.h \
    WorldResponder.h

__top_builddir__lib_libworldapi_a_SOURCES = \
    WorldMessage.cpp \
    RequestCreateWorld.cpp \
    RequestGetWorld.cpp \
    ResponseCreateWorld.cpp \
    ResponseError.cpp \
    ResponseGetWorld.cpp \
    WorldCache.cpp \
    WorldRequester.cpp \
    WorldResponder.cpp
