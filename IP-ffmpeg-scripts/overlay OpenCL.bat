call setpath
ffmpeg -threads 4 ^
-init_hw_device opencl=gpu:0.0 -filter_hw_device gpu ^
-i rtsp://192.168.100.150:8557/h264 ^
-fflags nobuffer -flags low_delay ^
-filter_complex "format=bgra,hwupload, avgblur_opencl=10, hwdownload" ^
-pix_fmt bgra -f sdl "Video"
pause

REM -filter_complex "[0:v]format=nv12[bg]; [1:v]format=nv12,hwupload[fg]; [bg][fg]overlay_opencl=0:0, hwdownload, format=nv12" ^