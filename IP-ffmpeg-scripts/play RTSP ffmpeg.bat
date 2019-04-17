call setpath
ffmpeg -hwaccel nvdec -threads 2 ^
-flags low_delay -fflags nobuffer ^
-i rtsp://192.168.100.150:8557/h264 ^
-pix_fmt bgra -f sdl "Video"
