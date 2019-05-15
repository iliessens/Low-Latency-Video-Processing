# Low Latency Video Processing & Annotation
Master's Thesis University of Antwerp
Imre Liessens
___

This folder contains all code files written for the thesis. They are ordered in different folders. Some folders contain an additionnal *readme* file for additional information.  

## Overview
Development of the C++ applications was done using Visual Studio 2017. Project files are provided.  

| Folder | Description |
|--------|--------|
|BMD-multiview|Similar to *BMD-splitscreen* but the video's are scaled so they can be shown in full. The empty space is filled with black, but could be changed to show a  background image.
|BMD-software-overlay| Own implementation of an overlay algorithm for use with capture cards of Blackmagic Design. I/O-devices and formats are configurable in <tt>settings.h</tt>|
|BMD-software-overlay-screen| Same as above, but the output is sent to the screen using the SDL2-library.|
|BMD-splitsceen|Split-screen implementation for Blackmagic Design Decklink devices|
|CasparCG-config|Contains the configuration file used for our experiments using [CasparCG](https://www.casparcg.com/). It also contains two rundown files, which can be used as examples.|
|IP-ffmpeg-scripts|A collection of different scripts for the FFmpeg tool, primarily for processing IP video from the camera.|
|IP-gstreamer-scripts|Some scripts for Gstreamer as an extra. This does not work very well.|
|Measurement|All files concerning the measurement solution, including code and a schematic. Consult the specific readme for more information.|

## Blackmagic SDK
The *BMD* applications make use of the Blackmagic Design Desktop Video SDK, which can be obtained from [their website](https://www.blackmagicdesign.com/developer/product/capture-and-playback). The include folder must be placed one level above this Readme file.  
Additionally, a compatible product and its drivers must be installed.
