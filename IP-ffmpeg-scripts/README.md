<style>
.bmd {background-color: lightgreen; padding: 2px}
.opencl {background-color: lightblue; padding: 2px}
</style>

# FFmpeg scripts
Download and unzip ffmpeg from the official website and unzip them somewhere on your PC.  
https://ffmpeg.zeranoe.com/builds/  

Edit the script ``setpath.bat`` to reflect your path to the executable location.  
If ffmpeg is already on your path, you can leave the script empty.  

Development has been done using version ``20190225-f948082``.  
However, some scripts require special features not available in the downloadable version. This is due to licensing constraints, which do not allow distribution of a compiled binary. To use these scripts, you need to compile FFmpeg yourself.  

These additional dependencies are indicated by the badges:
<span class="opencl">OpenCL support</span>  
<span class="bmd">BMD support</span>  
Libraries available in the default download are not listed.

The batch scripts are written on windows, but the options used should also work on other platforms.  

## Overview of the scripts
The top folder contains two scripts:
* setpath: adds the ffmpeg folder to path temporarily. This allows to easily change the ffmpeg binary version or location used for all scripts.
* ffmpeg_configure: this bash script (= Linux or MinGW shell!) contains the build commands for our custom version for FFmpeg. Note that this only sets the options, you still need to provide the external libraries yourself. The paths will probably need editing.  

Following scripts are present in the subfolders.  
### *ffplay*-scripts
These scripts use the ffplay tool to play an incoming video feed.  
There are three scripts available:  
* RTSP H.264
* RTSP MJPEG
* MPEG-TS H.264

Note that each of them requires a different camera configuration to function optimally.  

### Overlay
Scripts to add an overlay to a video feed. The overlay is always the Targa file present in the folder, but could be changed to anything else without much effort.  

* BMD overlay takes in a feed from a capture card (Blackmagic Design), adds an overlay and outputs it again through a playback card. Device names might need to be edited. <span class="bmd">BMD support</span>
* Overlay OpenCL should use an OpenCL capable graphics card to calculate the overlay.  However, during our experiments this did not work. Note that OpenCL support is not available in the distributed binaries. <span class="opencl">OpenCL support</span>  
* Overlay RTSP: adds an overlay to the RTSP video feed. More information about the choices made to write this script can be found in the final paper.  
* RTSP PiP: adds a prerecorded video on top of the live video feed to demonstrate a picture-in-picture. The path of the video and border color can be edited to match your preferences.  

### Splitscreen
Scripts to create a splitscreen composition. Because we only have on camera at our disposal, the second video is provided by a recording of our camera.  
* Splitscreen ffplay uses ffplay to play the video, which is a little more stable and user friendly, but a lot slower.  
* Splitscreen img: a splitscreen of the live video and a static image.
* Splitscreen rec: a splitscreen of the live video and a recorded video

### Tools
A collection of useful scripts that do not fit in one of the previous categories.

* Dump stream: Record a stream to disk without changing the data. The videostream is demuxed and recorded to a file. Thus, the encoding is still the one done by the camera.
* IP to BMD: receive the stream and play it through the output of a Blackmagic capture card <span class="bmd">BMD support</span>
* OpenCL demo: implements a blur operation using OpenCL <span class="opencl">OpenCL support</span>  
* Play cuvid: uses the Nvidia Cuvid API to decode the stream and display it using SDL2.
* Play RTSP ffmpeg: similar to the ffplay script, this plays the stream using the ffmpeg command and its SDL2 output. Not that ffplay also utilizes SDL2 for the display part.
* Probe RTSP: runs the ffprobe command on the RTSP stream, which allows for detailed data of the stream to be shown. Can be used to view the type of frames (P-, I- or B-frames).
