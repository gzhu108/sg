AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++17 \
    -I../../tools/json \
    -I../../tools/openssl/linux/include \
    -I../../microreactor/src

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = \
    $(top_builddir)/bin/bouncer

__top_builddir__bin_bouncer_SOURCES = \
    main.cpp \
    BouncerProfile.cpp \
    BouncerDispatcher.cpp \
    BouncerReactor.cpp \
    BouncerRestService.cpp
    
__top_builddir__bin_bouncer_LDADD = \
    ../../microreactor/lib/libmicroreactor.a
