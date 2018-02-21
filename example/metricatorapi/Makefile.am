AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++11 \
    -I../../microreactor/src

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
lib_LIBRARIES = \
    $(top_builddir)/lib/libmetricatorapi.a

__top_builddir__lib_libmetricatorapi_a_SOURCES = \
    MetricatorRequester.cpp \
    MetricatorRequest.cpp \
    MetricatorLogger.cpp
