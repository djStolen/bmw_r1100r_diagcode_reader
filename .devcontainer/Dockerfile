FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install core tools, build essentials, and Python (needed by many Arduino cores/tools)
RUN apt-get update && apt-get install -y \
    curl \
    git \
    build-essential \
    python3 \
    python3-pip \
    unzip \
    xz-utils \
    libncurses5 \
    && rm -rf /var/lib/apt/lists/*

# Install the latest Arduino CLI to /usr/local/bin
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

# Initialize the CLI and install the Nano cores
RUN arduino-cli core update-index && \
    arduino-cli core install arduino:avr && \
    arduino-cli core install arduino:megaavr && \
    arduino-cli core install arduino:samd && \
    arduino-cli core install arduino:mbed_nano

# Bake the TM1637 driver directly into the image
RUN arduino-cli lib install TM1637

# Set the default shell to bash
ENV SHELL=/bin/bash
