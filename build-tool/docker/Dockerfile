#
# Copyright © 2022 Arm Ltd and Contributors. All rights reserved.
# SPDX-License-Identifier: MIT
#

# Default build type is 'production'. Use 'dev' if supplying custom Arm NN / ACL repos from host
ARG BUILD_TYPE=production

ARG UBUNTU_VERSION=18.04
FROM ubuntu:${UBUNTU_VERSION} AS build-production

ENV DEBIAN_FRONTEND noninteractive

RUN sed -i "s@http://.*archive.ubuntu.com@http://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list

RUN sed -i "s@http://.*security.ubuntu.com@http://mirrors.tuna.tsinghua.edu.cn@g" /etc/apt/sources.list

# Install basic packages for Docker container (not specific to Arm NN)
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        ca-certificates \
        locales \
        vim \
        && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set locale for Docker container
RUN locale-gen en_GB.UTF-8 && \
    update-locale LC_ALL=en_GB.UTF-8 LANG=en_GB.UTF-8
ENV LANG en_GB.UTF-8
ENV LC_ALL en_GB.UTF-8

WORKDIR /root

# Install system-wide packages specific to Arm NN
COPY ./scripts/install-packages.sh .
COPY ./cmake-3.19.0 /root/cmake-3.19.0
RUN ./install-packages.sh

# Define user for non-root processes (overwriteable during 'docker build' with --build-arg)
ARG USER_ID=1000
ARG GROUP_ID=1000

# Create non-root user 'arm-user' based on $USER_ID and $GROUP_ID above
RUN addgroup --gid $GROUP_ID arm-user
RUN useradd --create-home --shell /bin/bash --uid $USER_ID --gid $GROUP_ID arm-user

# Switch to non-root user
USER arm-user
WORKDIR /home/arm-user

# Copy scripts required by Setup into WORKDIR
COPY --chown=arm-user:arm-user ./scripts/validation.sh .
COPY --chown=arm-user:arm-user ./scripts/common.sh .
COPY --chown=arm-user:arm-user ./scripts/setup-armnn.sh .
COPY --chown=arm-user:arm-user ./protobuf-3.12.0 ./source/protobuf-3.12.0
COPY --chown=arm-user:arm-user ./onnx-1.6.0 ./source/onnx-1.6.0
COPY --chown=arm-user:arm-user ./gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf ./gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf

# Run setup-armnn.sh: download and install Arm NN dependencies
ARG SETUP_ARGS=""
RUN echo "SETUP_ARGS: $SETUP_ARGS"
RUN ./setup-armnn.sh $SETUP_ARGS

# This build-dev stage (inherits 'build-production' stage) is only used in final image if $BUILD_TYPE is 'dev'
FROM build-production as build-dev

# Create directory for source repos in WORKDIR
RUN mkdir -p source/armnn source/acl

# Copy custom armnn/acl source repos from the build-tool directory on the host, if they exist (optional)
# If repos not provided, the build-armnn.sh script will automatically download the latest release branches of Arm NN and ACL
# Copies Dockerfile to ensure COPY works - at least one file must exist for COPY to work
COPY --chown=arm-user:arm-user ./docker/Dockerfile ./armnn* ./source/armnn/
COPY --chown=arm-user:arm-user ./docker/Dockerfile ./acl* ./source/acl/

# Final stage which inherits either 'build-production' or 'build-dev' stage
FROM build-${BUILD_TYPE} as final

# Copy build script into WORKDIR
COPY --chown=arm-user:arm-user ./scripts/build-armnn.sh .

# Run build-armnn.sh: build Arm NN and ACL
ARG BUILD_ARGS=""
RUN echo "BUILD_ARGS: $BUILD_ARGS"
RUN ./build-armnn.sh $BUILD_ARGS