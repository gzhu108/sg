AUTOMAKE_OPTIONS = foreign

# what flags you want to pass to the C compiler & linker
AM_CFLAGS = -Wall -Wno-unknown-pragmas -Wno-unused-result -g -fno-omit-frame-pointer -pthread -pipe
AM_CXXFLAGS = $(AM_CFLAGS) -Wno-reorder -std=c++17 \
    -Isrc \
    -I../tools/rapidxml \
    -I../tools/googletest/googletest/include \
    -I../tools/openssl/linux/include \
    -I../tools/http-parser

AM_LDFLAGS = -static-libstdc++

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
lib_LIBRARIES = $(top_builddir)/lib/libmicroreactor.a

nobase_include_HEADERS = \
    src/BinarySerializer.h \
    src/Cancelable.h \
    src/Client.h \
    src/Common.h \
    src/Comparable.h \
    src/Configuration.h \
    src/ConfigurationXml.h \
    src/Connection.h \
    src/Dispatcher.h \
    src/Emittable.h \
    src/Endianness.h \
    src/Exception.h \
    src/Endpoint.h \
    src/ConfigurationSingleton.h \
    src/LoggerSingleton.h \
    src/MemoryTrack.h \
    src/MessageDispatcher.h \
    src/Message.h \
    src/MessageReactor.h \
    src/MessageRequester.h \
    src/MessageResponder.h \
    src/Microreactor.h \
    src/Microservice.h \
    src/NetworkUtility.h \
    src/Park.h \
    src/Parkable.h \
    src/Profile.h \
    src/Property.h \
    src/Reactor.h \
    src/ScopeLock.h \
    src/SecureTcpSocket.h \
    src/Serializable.h \
    src/Serializer.h \
    src/Service.h \
    src/Shareable.h \
    src/Socket.h \
    src/StringSerializer.h \
    src/StringUtility.h \
    src/TaskFuture.h \
    src/Task.h \
    src/TaskCategorizedQueue.h \
    src/TaskManager.h \
    src/TaskManagerSingleton.h \
    src/TaskProcessHook.h \
    src/TaskQueue.h \
    src/TcpConnection.h \
    src/TcpEndpoint.h \
    src/TcpSocket.h \
    src/ThreadPool.h \
    src/UdpConnection.h \
    src/UdpEndpoint.h \
    src/UdpSocket.h \
    src/Uuid.h \
    src/HttpHeader.h \
    src/RestFileResponse.h \
    src/RestMessage.h \
    src/RestDispatcher.h \
    src/RestReactor.h \
    src/RestRequest.h \
    src/RestResponse.h \
    src/RestService.h \
    src/UriCodec.h

__top_builddir__lib_libmicroreactor_a_SOURCES = \
    src/BinarySerializer.cpp \
    src/Client.cpp \
    src/ConfigurationXml.cpp \
    src/Connection.cpp \
    src/Dispatcher.cpp \
    src/Endpoint.cpp \
    src/ConfigurationSingleton.cpp \
    src/LoggerSingleton.cpp \
    src/Message.cpp \
    src/MessageRequester.cpp \
    src/MessageResponder.cpp \
    src/Microservice.cpp \
    src/NetworkUtility.cpp \
    src/Profile.cpp \
    src/Reactor.cpp \
    src/Serializer.cpp \
    src/Service.cpp \
    src/Socket.cpp \
    src/StringSerializer.cpp \
    src/StringUtility.cpp \
    src/Task.cpp \
    src/TaskCategorizedQueue.cpp \
    src/TaskManager.cpp \
    src/TaskManagerSingleton.cpp \
    src/TaskQueue.cpp \
    src/TcpConnection.cpp \
    src/TcpEndpoint.cpp \
    src/TcpSocket.cpp \
    src/SecureTcpSocket.cpp \
    src/ThreadPool.cpp \
    src/UdpConnection.cpp \
    src/UdpEndpoint.cpp \
    src/UdpSocket.cpp \
    src/Uuid.cpp \
    src/RestMessage.cpp \
    src/RestRequest.cpp \
    src/RestResponse.cpp \
    src/RestFileResponse.cpp \
    src/RestDispatcher.cpp \
    src/RestService.cpp \
    ../tools/http-parser/http_parser.c

check_PROGRAMS = $(top_builddir)/test/bin/microreactor_test

__top_builddir__test_bin_microreactor_test_SOURCES = \
    test/test_main.cpp \
    test/UuidTest.cpp \
    test/RestDispatcherTest.cpp
    
__top_builddir__test_bin_microreactor_test_LDADD = \
    lib/libmicroreactor.a \
    ../tools/googletest/googlemock/libgmock.a \
    ../tools/googletest/googlemock/gtest/libgtest.a \
    -luuid

TESTS = $(check_PROGRAMS)
