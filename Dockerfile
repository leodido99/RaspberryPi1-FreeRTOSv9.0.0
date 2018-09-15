# use small debian base image
FROM debian:stretch-slim
# install the general build tools
RUN apt-get update && apt-get -y install build-essential vim-common wget git bzip2 make python libdb1-compat tzdata astyle clang cmake
# download the arm-none-eabi Toolchain
RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2018q2/gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 && \
# create directory to store the Toolchain
mkdir -p /usr/local/bin/ && \
# extract the toolchain into the directory created
tar -xf gcc-arm-none-eabi-7-2018-q2-update-linux.tar.bz2 -C /usr/local/bin/