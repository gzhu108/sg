AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++17 \
    -I../../tools/json \
    -I../../microreactor/src \
    -I.. \
    -I../discoveryservice \
    -I../worldapi \
    -I../streetgangapi \
    -I../streetgangproto/cpp \
    -I../metricatorapi

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = \
    $(top_builddir)/bin/streetgangserver

__top_builddir__bin_streetgangserver_SOURCES = \
    main.cpp \
    RequestByebyeReactor.cpp \
    RequestCreateWorldReactor.cpp \
    RequestGetSceneReactor.cpp \
    RequestGetVersionReactor.cpp \
    StreetGangBinaryDispatcher.cpp \
    StreetGangPBDispatcher.cpp \
    StreetGangBinaryService.cpp \
    StreetGangPBService.cpp \
    StreetGangRestDispatcher.cpp \
    StreetGangReactor.cpp \
    WorldClient.cpp \
    WorldClientDispatcher.cpp


__top_builddir__bin_streetgangserver_LDADD = \
    ../metricatorapi/lib/libmetricatorapi.a \
    ../streetgangapi/lib/libstreetgangapi.a \
    ../streetgangproto/lib/libstreetgangproto.a \
    ../worldapi/lib/libworldapi.a \
    ../discoveryservice/lib/libdiscoveryservice.a \
    ../../microreactor/lib/libmicroreactor.a \
    -lprotobuf \
    -lssl \
    -lcrypto \
    -luuid \
    -ldl

