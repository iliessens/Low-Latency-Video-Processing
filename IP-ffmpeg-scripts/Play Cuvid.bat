call setpath
ffmpeg -hwaccel cuvid -threads 2 ^
-c:v h264_cuvid ^
-max_delay 0 ^
-i rtsp://192.168.100.150:8557/h264 ^
-filter_complex "hwdownload,format=nv12" ^
-fflags nobuffer -flags low_delay ^
-pix_fmt bgra -f sdl "Video"

pause