#!/usr/bin/sh
set -x
cd ../../ffmpeg
PKG_CONFIG_PATH="nv-codec-headers/"
#--enable-opencl
./configure --enable-nvenc --enable-nvdec --enable-opengl --disable-shared --enable-static \
 --enable-decklink --extra-cflags=-I"decklink-headers/" --extra-ldflags=-L"decklink-headers/"
 make -j8