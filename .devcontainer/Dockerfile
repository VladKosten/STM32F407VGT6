# Start from ubuntu 22.04 image and
# install some tools for embedded development on arm cortex-m microcontrollers
FROM ubuntu:22.04

# Set the working directory
WORKDIR /usr/src/app

# Set the maintainer
LABEL maintainer="Vlad Kosten"

# Update the package list
RUN apt update

# add cpio
RUN apt-get install cpio libncurses5 -y

# gcc, g++ and make for building C/C++ projects for Host
RUN apt install -y build-essential

# For downloading files from the internet (e.g. downloading the ARM GCC toolchain)
RUN apt install -y wget

# Cmake and Ninja for building C/C++ projects
RUN apt install -y ninja-build
RUN apt install -y cmake

# For version control
RUN apt install -y git

WORKDIR /bin/.
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2020q2/gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2 -O gcc-arm-none-eabi.tar.bz2
RUN mkdir gcc-arm-none-eabi && tar xjfv gcc-arm-none-eabi.tar.bz2 -C ./gcc-arm-none-eabi --strip-components 1
RUN rm gcc-arm-none-eabi.tar.bz2
ENV PATH="${PATH}:/bin/gcc-arm-none-eabi/bin"

RUN apt update

# Install python3 and pip3
RUN apt install python3 python3-pip -y

# Install pyocd
RUN pip3 install --no-cache-dir --user -U pyocd

# ADD pyocd to PATH
ENV PATH=$PATH:/root/.local/bin

# Install the pack for the
RUN pyocd pack install stm32f407
