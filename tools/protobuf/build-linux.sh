#!/bin/bash

rm -rf linux

if [ ! -d "protobuf-3.5.1" ]; then
    if [ ! -f "protobuf-all-3.5.1.tar.gz" ]; then
        wget https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-all-3.5.1.tar.gz
    fi
    tar xvfz protobuf-all-3.5.1.tar.gz
fi

pushd protobuf-3.5.1
    ./configure --prefix=$PWD/../linux LDFLAGS=-static-libstdc++
    make
    make install
popd

