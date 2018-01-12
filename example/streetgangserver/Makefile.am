AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -O2 -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++11 \
    -I../../tools/protobuf/linux/include \
    -I../../tools/json \
    -I../../microreactor/src \
    -I../discoveryservice \
    -I../streetgangapi \
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
    RequestErrorReactor.cpp \
    RequestGetSceneReactor.cpp \
    RequestGetVersionReactor.cpp \
    StreetGangMessageDecoder.cpp \
    StreetGangPBDecoder.cpp \
    StreetGangService.cpp \
    StreetGangSessionManager.cpp \
    WorldServiceProvider.cpp \
    StreetGangRestService.cpp

__top_builddir__bin_streetgangserver_LDADD = \
    ../metricatorapi/lib/libmetricatorapi.a \
    ../streetgangapi/lib/libstreetgangapi.a \
    ../discoveryservice/lib/libdiscoveryservice.a \
    ../../microreactor/lib/libmicroreactor.a \
    ../../tools/protobuf/linux/lib/libprotobuf.a

