FROM ubuntu:latest AS pico-builder

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get install --no-install-recommends -y cmake \
    build-essential \
    git \
    apt-transport-https \
    ca-certificates \
    python3 \
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    && rm -rf /var/lib/apt/lists/*

# Keep PICO_SDK_PATH persistent in the image
ENV PICO_SDK_PATH=/opt/pico-sdk
ARG PICO_SDK_VERSION=1.0.0
RUN git clone --depth 1 --branch ${PICO_SDK_VERSION} \
    https://github.com/raspberrypi/pico-sdk \
    ${PICO_SDK_PATH}
