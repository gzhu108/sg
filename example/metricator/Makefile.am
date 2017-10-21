AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -O2 -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++11 \
    -I../../microreactor/src \
    -I../discoveryservice \
    -I../metricatorapi

AM_LDFLAGS =

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = \
    $(top_builddir)/bin/metricator

__top_builddir__bin_metricator_SOURCES = \
    main.cpp \
    MetricatorMessageDecoder.cpp \
    MetricatorRequestReactor.cpp
    
__top_builddir__bin_metricator_LDADD = \
    ../metricatorapi/lib/libmetricatorapi.a \
    ../discoveryservice/lib/libdiscoveryservice.a \
    ../../microreactor/lib/libmicroreactor.a
