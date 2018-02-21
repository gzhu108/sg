AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++11 \
    -I../../tools/protobuf/linux/include \
    -I../../microreactor/src \
    -I../streetgangapi

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = \
    $(top_builddir)/bin/streetgangclient

__top_builddir__bin_streetgangclient_SOURCES = \
    main.cpp \
    ResponseCreateWorldReactor.cpp \
    ResponseErrorReactor.cpp \
    ResponseGetSceneReactor.cpp \
    ResponseGetVersionReactor.cpp \
    StreetGangBinaryClient.cpp \
    StreetGangBinaryClientDispatcher.cpp \
    StreetGangPBClient.cpp \
    StreetGangPBClientDispatcher.cpp

__top_builddir__bin_streetgangclient_LDADD = \
    ../streetgangapi/lib/libstreetgangapi.a \
    ../../microreactor/lib/libmicroreactor.a \
    ../../tools/protobuf/linux/lib/libprotobuf.a \
    -luuid
    