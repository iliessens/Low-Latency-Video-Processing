#!/usr/bin/sh
#Print commands
set -x
#Fail on errors
set -e
cd ../../ffmpeg

PKG_CONFIG_PATH="nv-codec-headers/"
./configure --enable-nvenc --enable-nvdec --enable-opengl --enable-opencl --enable-sdl --disable-shared --enable-static \
--extra-cflags=-I"opencl/include/" --extra-ldflags=-L"opencl/lib/"
 make -j8