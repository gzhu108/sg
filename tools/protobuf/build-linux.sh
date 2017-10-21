#!/bin/sh

rm -rf protobuf-3.1.0
unzip protobuf-cpp-3.1.0.zip
cd protobuf-3.1.0
./configure --prefix=$PWD/../linux LDFLAGS=-static-libstdc++
make
make install
