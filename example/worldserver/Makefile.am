AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++17 \
    -I../../microreactor/src \
    -I../worldapi

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = \
    $(top_builddir)/bin/worldserver

__top_builddir__bin_worldserver_SOURCES = \
    main.cpp \
    CreateWorldReactor.cpp \
    GetWorldReactor.cpp \
    WorldManager.cpp \
    WorldProvider.cpp \
    WorldDispatcher.cpp \
    WorldService.cpp 

__top_builddir__bin_worldserver_LDADD = \
    ../worldapi/lib/libworldapi.a \
    ../../microreactor/lib/libmicroreactor.a
