FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install core tools, build essentials, and sudo
RUN apt-get update && apt-get install -y \
    curl \
    git \
    build-essential \
    python3 \
    python3-pip \
    unzip \
    xz-utils \
    libncurses5 \
    sudo \
    picocom \
    && rm -rf /var/lib/apt/lists/*

# Explicitly create the 'ubuntu' user with a home directory and bash shell
RUN useradd -rm -d /home/ubuntu -s /bin/bash -g root -G sudo,dialout -u 1000 ubuntu

# Allow the 'ubuntu' user to run sudo commands without prompting for a password
RUN echo "ubuntu ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# FIX: Download and extract the absolute 64-bit Linux binary directly (v1.1.1)
RUN curl -fsSL https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_64bit.tar.gz -o arduino-cli.tar.gz && \
    tar -xf arduino-cli.tar.gz -C /usr/local/bin/ arduino-cli && \
    rm arduino-cli.tar.gz

# Pre-create the target workspace directory and give ownership to 'ubuntu'
RUN mkdir -p /workspace && chown -R ubuntu:root /workspace

# Switch to the non-root 'ubuntu' user context for installing cores and libraries
USER ubuntu
WORKDIR /home/ubuntu

# Initialize CLI and download configurations under the 'ubuntu' user context
RUN arduino-cli core update-index && \
    arduino-cli core install arduino:avr && \
    arduino-cli core install arduino:megaavr && \
    arduino-cli core install arduino:samd && \
    arduino-cli core install arduino:mbed_nano

# Pre-install the TM1637 driver for your display setup
RUN arduino-cli lib install TM1637

# Reset workspace context for when the workspace mounts your repository files
WORKDIR /workspace
ENV SHELL=/bin/bash
