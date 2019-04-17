#!/usr/bin/sh
set -x
cd ../../ffmpeg
PKG_CONFIG_PATH="nv-codec-headers/"
./configure --enable-nvenc --enable-nvdec --enable-opengl --enable-opencl --enable-sdl --disable-shared --enable-static \
 --enable-decklink --extra-cflags=-I"decklink-headers/" --extra-ldflags=-L"decklink-headers/"
 make -j8