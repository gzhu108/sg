FROM amazonlinux:latest

RUN yum update -y

RUN yum install -y gcc gcc-c++ make curl unzip tar wget git openssl-devel
#RUN yum install -y cmake

WORKDIR /git
RUN wget https://github.com/Kitware/CMake/releases/download/v3.17.1/cmake-3.17.1.tar.gz
RUN tar -xvzf cmake-3.17.1.tar.gz
WORKDIR /git/cmake-3.17.1
RUN ./bootstrap
RUN make
RUN make install

WORKDIR /git
RUN rm -rf /git/cmake-3.17.1
RUN rm cmake-3.17.1.tar.gz
RUN yum remove -y openssl-devel

RUN git clone https://github.com/Microsoft/vcpkg.git
WORKDIR /git/vcpkg
RUN ./bootstrap-vcpkg.sh
RUN ./vcpkg integrate install

RUN ./vcpkg install gtest http-parser openssl protobuf rapidjson rapidxml libuuid
RUN source ./scripts/vcpkg_completion.bash

ENV VCPKG_DEFAULT_TRIPLET=x64-linux
ENV VCPKG_ROOT=/git/vcpkg
