FROM amazonlinux:latest

RUN yum update -y

RUN yum install -y gcc
RUN yum install -y gcc-c++
RUN yum install -y make
RUN yum install -y cmake
RUN yum install -y curl unzip tar
RUN yum install -y git

WORKDIR /git
RUN git clone https://github.com/Microsoft/vcpkg.git

WORKDIR /git/vcpkg
RUN ./bootstrap-vcpkg.sh
RUN ./vcpkg integrate install

RUN ./vcpkg install gtest http-parser openssl protobuf rapidjson rapidxml libuuid
RUN source ./scripts/vcpkg_completion.bash

ENV VCPKG_DEFAULT_TRIPLET=x64-linux
ENV VCPKG_ROOT=/git/vcpkg
