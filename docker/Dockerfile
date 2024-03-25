FROM ilcardella/rpi-pico-sdk:latest AS builder

# Install lib_coffee_machine
ARG LCF_PATH=/opt/lib_coffee_machine
ARG LCF_VERSION=main
RUN git clone --depth 1 --branch ${LCF_VERSION} \
    https://github.com/ilcardella/lib_coffee_machine \
    ${LCF_PATH} \
    && make -C ${LCF_PATH} install
