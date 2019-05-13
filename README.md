# Low Latency Video Processing & Annotation
Master's thesis University of Antwerp
-------

This folder contains all code files written for the thesis. They are ordered in different folders. Some folders contain an additionnal *readme* file for additional information.  

## Overview

| Folder | Description |
|--------|--------|
|BMD-software-overlay| Own implementation of an overlay algorithm for use with capture cards of Blackmagic Design. I/O-devices and formats are configurable in <tt>settings.h</tt>|
|BMD-software-overlay-screen| Same as above, but the output is sent to the screen using the SDL2-library.|
|BMD-splitsceen|Split-screen implementation for Blackmagic Design Decklink devices|
|CasparCG-config|Contains the configuration file used for our experiments using [CasparCG](https://www.casparcg.com/)|
|IP-ffmpeg-scripts|A collection of different scripts for the FFmpeg tool, primarily for processing IP video from the camera.|
|IP-gstreamer-scripts|Some scripts for Gstreamer as an extra. This does not work very well.|
|Measurement|All files concerning the measurement solution, including code and a schematic. Consult the specific readme for more information.|
