call setpath
ffmpeg -hwaccel nvdec -threads 2 ^
-i rtsp://192.168.100.150:8557/h264 ^
-fflags nobuffer -flags low_delay ^
-pix_fmt bgra -f sdl "Video"