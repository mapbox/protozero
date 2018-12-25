# Minimal mapbox/protozero build, test and install
# using Docker Ubuntu 16.04 (Xenial)

FROM ubuntu:xenial
MAINTAINER Nigel Stewart (nigels@nigels.com)

#
# Optional custom package mirror
#
# --build-arg MIRROR=http://au.archive.ubuntu.com/ubuntu/
#

ARG MIRROR
ENV MIRROR ${MIRROR:-http://archive.ubuntu.com/ubuntu}

RUN sed -i -r "/#.*/d"                                           /etc/apt/sources.list
RUN sed -i -r "/^$/d"                                            /etc/apt/sources.list
RUN cat /etc/apt/sources.list
RUN sed -i -r "s#deb .* xenial(-.*)? #deb ${MIRROR} xenial\\1 #" /etc/apt/sources.list
RUN cat /etc/apt/sources.list

#
# Update and install build tools, dependencies
#

ENV DEBIAN_FRONTEND noninteractive

RUN apt update
RUN apt dist-upgrade -y

RUN apt install -y build-essential git cmake doxygen graphviz cppcheck
RUN apt install -y libprotobuf-dev protobuf-compiler

#
# Clone repo
#

ENV SCRATCH /scratch
RUN mkdir ${SCRATCH}
WORKDIR ${SCRATCH}

ARG REPO
ENV REPO ${REPO:-https://github.com/mapbox/protozero.git}
RUN git clone ${REPO} repo

#
# Build
#

RUN mkdir -p ${SCRATCH}/repo/build
WORKDIR ${SCRATCH}/repo/build
RUN cmake ..
RUN make

#
# Test
#

RUN ctest

#
# Install
#

RUN make install
RUN find /usr/local
