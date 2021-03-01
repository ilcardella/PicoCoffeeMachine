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
ARG PICO_SDK_VERSION=1.0.1
RUN git clone --depth 1 --branch ${PICO_SDK_VERSION} \
    https://github.com/raspberrypi/pico-sdk \
    ${PICO_SDK_PATH}

# Install lib_coffee_machine
ARG LCF_PATH=/opt/lib_coffee_machine
ARG LCF_VERSION=v0.2.0
RUN git clone --depth 1 --branch ${LCF_VERSION} \
    https://github.com/ilcardella/lib_coffee_machine \
    ${LCF_PATH} \
    && make -C ${LCF_PATH} install

# Install ssd1306_ascii_pico library
ARG SSD1306_PATH=/opt/ssd1306_ascii_pico
ARG SSD1306_VERSION=v0.2.0
RUN git clone --depth 1 --branch ${SSD1306_VERSION} \
    https://github.com/ilcardella/ssd1306_ascii_pico \
    ${SSD1306_PATH} \
    && make -C ${SSD1306_PATH} install
