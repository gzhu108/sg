AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++17 \
    -I../../tools/json \
    -I../../microreactor/src \
    -Igenerated

AM_LDFLAGS = -static-libstdc++ -L/usr/lib

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = \
    $(top_builddir)/bin/myserver

__top_builddir__bin_myserver_SOURCES = \
    generated/GETv1versionMessage.cpp \
    generated/GETv1versionResponse.cpp \
    generated/GETv1versionReactorBase.cpp \
    generated/POSTv1updateMessage.cpp \
    generated/POSTv1updateResponse.cpp \
    generated/POSTv1updateReactorBase.cpp \
    generated/MyServerServiceBase.cpp \
    MyService.cpp \
    VersionReactor.cpp \
    UpdateReactor.cpp \
    main.cpp

__top_builddir__bin_myserver_LDADD = \
    ../../microreactor/lib/libmicroreactor.a \
    ../../tools/openssl/linux/lib/libssl.a \
    ../../tools/openssl/linux/lib/libcrypto.a \
    -luuid \
    -ldl
